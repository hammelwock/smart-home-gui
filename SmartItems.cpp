#include "SmartItems.h"
#include <QStack>
#include <QDebug>

#include "Core.h"


Sensor::Sensor(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();
    type = json["type"].toString();
    pin = json["pin"].toInt();
    refreshRate = json["refreshRate"].toInt();
    measuredQuantity = json["unit"].toString();

    value = 0;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sensor::refreshValue);
    timer->start(refreshRate);

    connect(comPortAdapter, &ComPortAdapter::valueRead, this, &Sensor::readValue);

    allElements->addSensor(this);
}

QJsonObject Sensor::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;
    obj["type"] = type;
    obj["pin"] = pin;
    obj["refreshRate"] = refreshRate;
    obj["measuredQuantity"] = measuredQuantity;

    return obj;
}

void Sensor::refreshValue()
{
    QJsonObject json;
    json["cmd"] = "read";
    json["type"] = type;
    json["pin"] = pin;
    comPortAdapter->sendJson(json);
}

void Sensor::readValue(int pin, double value, QString type)
{
    if (pin == this->pin && type == this->type)
    {
        this->value = value;
        emit valueChanged();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Regulator::Regulator(const QJsonObject &json)
{
    target = json["target"].toInt();
    hysteresis = json["hysteresis"].toInt();
    invert = json["invert"].toBool();
    sensorName = json["sensorName"].toString();

    connect(allElements, &AllElements::sensorListChanged, this, &Regulator::setSensor);
}

void Regulator::regulate()
{
    if (state)
        state = target - hysteresis / 2 > sensor->getValue();
    else
        state = target + hysteresis / 2 > sensor->getValue();

    if(invert)
        state = !state;

    emit adjusted(state);
}

void Regulator::setSensor()
{
    sensor = allElements->getSensor(sensorName);
    connect(sensor, &Sensor::valueChanged, this, &Regulator::regulate);
}

QJsonObject Regulator::saveJson()
{
    QJsonObject obj;

    obj["target"] = target;
    obj["hysteresis"] = hysteresis;
    obj["state"] = state;
    obj["invert"] = invert;
    obj["sensorName"] = sensorName;

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

    if (json.contains("regulator"))
        regulator = new Regulator(json["regulator"].toObject());

    comPortAdapter->sendJson(initPinJson);
    connect(this, &Actuator::valueChanged, this, &Actuator::sendValue);
    connect(regulator, &Regulator::adjusted, this, &Actuator::setValue); //to do?
}

void Actuator::setValue(int value)
{
    if (this->value != value)
    {
        this->value = value;
        emit valueChanged();
    }
}


QJsonObject Actuator::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;
    obj["type"] = type;
    obj["pin"] = pin;

    if(regulator)
        obj["regulator"] = regulator->saveJson();

    return obj;
}


void Actuator::sendValue()
{
    QJsonObject json;
    json["cmd"] = "digitalWrite";
    json["pin"] = pin;
    json["setValue"] = value;
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
    obj["actuatorList"] = actuatorArray;

    return obj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Controller::Controller(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();
    type = json["type"].toString();

    QJsonArray smartItems = json["smartItemList"].toArray();
    for (const QJsonValue &value : smartItems)
        smartItemList.append(new SmartItem(value.toObject(), comPortAdapter));
}

QJsonObject Controller::saveJson()
{
    QJsonObject obj;

    obj["name"] = name;
    obj["type"] = type;

    QJsonArray smartItemArray;
    for (int i = 0; i < smartItemList.count(); i++)
        smartItemArray.append(qobject_cast<SmartItem*>(smartItemList.at(i))->saveJson());
    obj["smartItemList"] = smartItemArray;

    comPortAdapter->saveConfig(obj);

    return obj;
}
