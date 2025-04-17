#include "SmartItems.h"
#include <QStack>
#include <QDebug>

#include "Core.h"


Sensor::Sensor(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();
    type = json["sensorType"].toString();
    pin = json["pin"].toInt();
    index = json["index"].toInt();
    refreshRate = json["refreshRate"].toInt();
    measuredQuantity = json["unit"].toString();

    value = 0;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sensor::refreshValue);
    timer->start(refreshRate);

    connect(comPortAdapter, &ComPortAdapter::ds18b20Read, this, &Sensor::readValue);

    allElements->addSensor(this);
}

QJsonObject Sensor::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;
    obj["type"] = type;
    obj["pin"] = pin;
    obj["index"] = index;
    obj["refreshRate"] = refreshRate;
    obj["measuredQuantity"] = measuredQuantity;

    return obj;
}

void Sensor::refreshValue()
{
    QJsonObject json;
    json["cmd"] = "read";
    json["type"] = type;
    json["index"] = index;
    comPortAdapter->sendJson(json);
}

void Sensor::readValue(int index, double value)
{
    if (index == this->index)
    {
        this->value = value;
        emit valueChanged();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Regulator::Regulator(int target, int hysteresis, bool invert)
{
    this->target = target;
    this->hysteresis = hysteresis;
    this->invert = invert;
}

bool Regulator::regulate()
{
    if (state)
        state = target - hysteresis / 2 > sensor->getValue();
    else
        state = target + hysteresis / 2 > sensor->getValue();

    if(invert)
        state = !state;

    return state;
}

QJsonObject Regulator::saveJson()
{
    QJsonObject obj;

    obj["target"] = target;
    obj["hysteresis"] = hysteresis;
    obj["state"] = state;
    obj["invert"] = invert;
    obj["sensorName"] = sensor->getName();

    return obj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Actuator::Actuator(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();
    type = json["actuatorType"].toInt();
    pin = json["pin"].toInt();
    value = 0;

    QJsonObject initPinJson;
    initPinJson["cmd"] = "pinMode";
    initPinJson["pin"] = pin;
    initPinJson["mode"] = "output";

    comPortAdapter->sendJson(initPinJson);
    connect(this, &Actuator::valueChanged, this, &Actuator::sendLevl);
}


QJsonObject Actuator::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;
    obj["type"] = type;
    obj["pin"] = pin;

    obj["regulator"] = regulator->saveJson();

    return obj;
}


void Actuator::sendLevl()
{
    QJsonObject json;
    json["cmd"] = "write";
    json["pin"] = pin;
    json["value"] = value;
    comPortAdapter->sendJson(json);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SmartItem::SmartItem(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();

    QJsonArray sensors = json["sensorList"].toArray();
    for (const QJsonValue &value : sensors)
        sensorList.append(new Sensor(value.toObject(), comPortAdapter));

    QJsonArray actuators = json["actuatorList"].toArray();
    for (const QJsonValue &value : actuators)
        actuatorList.append(new Actuator(value.toObject(), comPortAdapter));
}

QJsonObject SmartItem::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;

    QJsonArray sensorArray;
    for (int i = 0; i < sensorList.count(); i++)
        sensorArray.append(qobject_cast<Sensor*>(sensorList.at(i))->saveJson());
    obj["sensorList"] = sensorArray;

    QJsonArray actuatorArray;
    for (int i = 0; i < actuatorList.count(); i++)
        actuatorArray.append(qobject_cast<Actuator*>(actuatorList.at(i))->saveJson());
    obj["smartItemList"] = actuatorArray;

    return obj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Room::Room(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();

    QJsonArray smartItems = json["smartItemList"].toArray();
    for (const QJsonValue &value : smartItems)
        smartItemList.append(new SmartItem(value.toObject(), comPortAdapter));

    QJsonArray sensors = json["sensorList"].toArray();
    for (const QJsonValue &value : sensors)
        sensorList.append(new Sensor(value.toObject(), comPortAdapter));
}

QJsonObject Room::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;

    QJsonArray smartItemArray;
    for (int i = 0; i < smartItemList.count(); i++)
        smartItemArray.append(qobject_cast<SmartItem*>(smartItemList.at(i))->saveJson());
    obj["smartItemList"] = smartItemArray;

    QJsonArray sensorArray;
    for (int i = 0; i < sensorList.count(); i++)
        sensorArray.append(qobject_cast<Sensor*>(sensorList.at(i))->saveJson());
    obj["sensorList"] = sensorArray;

    return obj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Home::Home(QString config, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    QJsonDocument doc = QJsonDocument::fromJson(config.toUtf8());
    QJsonObject root = doc.object();

    QJsonObject home = root["home"].toObject();

    QJsonArray rooms = home["roomList"].toArray();
    qDebug() << "rooms содержит:" << rooms;
    for (const QJsonValue &value : rooms)
        roomList.append(new Room(value.toObject(), comPortAdapter));

    QJsonArray sensors = home["sensorList"].toArray();
    for (const QJsonValue &value : sensors)
        sensorList.append(new Sensor(value.toObject(), comPortAdapter));

}

void Home::saveJson()
{
    QJsonObject obj;

    QJsonArray roomArray;
    for (int i = 0; i < roomList.count(); i++)
        roomArray.append(qobject_cast<SmartItem*>(roomList.at(i))->saveJson());
    obj["roomList"] = roomArray;

    QJsonArray sensorArray;
    for (int i = 0; i < sensorList.count(); i++)
        sensorArray.append(qobject_cast<Sensor*>(sensorList.at(i))->saveJson());
    obj["sensorList"] = sensorArray;

}
