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

    onPinChanged();

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sensor::refreshValue);
    timer->start(refreshRate);

    connect(comPortAdapter, &ComPortAdapter::valueRead, this, &Sensor::readValue);

    allElements->addSensor(this);
}


Sensor::Sensor(ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = "имя";
    type = "тип";
    pin = 0;
    refreshRate = 1000;
    measuredQuantity = "unit";
    value = 0;

    onPinChanged();

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sensor::refreshValue);
    timer->start(refreshRate);

    connect(comPortAdapter, &ComPortAdapter::valueRead, this, &Sensor::readValue);

    allElements->addSensor(this);
}


void Sensor::setName(QString name)
{
    if (this->name != name)
    {
        this->name = name;
        emit paramChanged();
    }
}


void Sensor::setType(QString type)
{
    if (this->type != type)
    {
        this->type = type;
        emit paramChanged();
    }
}


void Sensor::setPin(int pin)
{
    if (this->pin != pin)
    {
        this->pin = pin;
        emit paramChanged();
    }
}


void Sensor::setRefreshRate(int refreshRate)
{
    if (this->refreshRate != refreshRate)
    {
        this->refreshRate = refreshRate;
        emit paramChanged();
    }
}


void Sensor::setMeasuredQuantity(QString measuredQuantity)
{
    if (this->measuredQuantity != measuredQuantity)
    {
        this->measuredQuantity = measuredQuantity;
        emit paramChanged();
    }
}


void Sensor::remove()
{
    emit removeMe(this);
    delete this;
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


void Sensor::onPinChanged()
{
    if(type != "ds18b20")
    {
        QJsonObject initPinJson;
        initPinJson["cmd"] = "pinMode";
        initPinJson["pin"] = pin;
        initPinJson["mode"] = "input";

        comPortAdapter->sendJson(initPinJson);
    }
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
    setSensor();
}


Regulator::Regulator()
{
    target = 0;
    hysteresis = 0;
    invert = false;
    sensorName = "сенсор";

    connect(allElements, &AllElements::sensorListChanged, this, &Regulator::setSensor);
    setSensor();
}


void Regulator::setTarget(int target)
{
    if (this->target != target)
    {
        this->target = target;
        emit paramChanged();
    }
}


void Regulator::setHysteresis(int h) {
    if (hysteresis == h) return;
    hysteresis = h;
    emit paramChanged();
}


void Regulator::setInvert(bool i) {
    if (invert == i) return;
    invert = i;
    emit paramChanged();
}


void Regulator::setSensorName(const QString &name) {
    if (sensorName == name) return;
    sensorName = name;
    emit paramChanged();
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
    type = json["type"].toString();
    pin = json["pin"].toInt();
    value = 0;

    onPinChanged();

    if (json.contains("regulator"))
        regulator = new Regulator(json["regulator"].toObject());
    else
        regulator = 0;

    connect(this, &Actuator::valueChanged, this, &Actuator::sendValue);
    //connect(regulator, &Regulator::adjusted, this, &Actuator::setValue);//to do
}


Actuator::Actuator(ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = "имя";
    type = "тип";
    pin = 0;
    value = 0;
    regulator = 0;

    onPinChanged();

    connect(this, &Actuator::valueChanged, this, &Actuator::sendValue);
    //connect(regulator, &Regulator::adjusted, this, &Actuator::setValue);//to do
}

Actuator::~Actuator()
{
    delete regulator;
}


void Actuator::setName(QString name)
{
    if (this->name != name)
    {
        this->name = name;
        emit paramChanged();
    }
}


void Actuator::setType(QString type)
{
    if (this->type != type)
    {
        this->type = type;
        emit paramChanged();
    }
}


void Actuator::setPin(int pin)
{
    if (this->pin != pin)
    {
        this->pin = pin;
        onPinChanged();
        emit paramChanged();
    }
}


void Actuator::setValue(int value)
{
    if (this->value != value)
    {
        this->value = value;
        emit valueChanged();
    }
}


void Actuator::remove()
{
    emit removeMe(this);
    delete this;
}


void Actuator::addRegulator()
{
    if (regulator)
        delete regulator;

    regulator = new Regulator();
    emit regulatorChanged();
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


void Actuator::onPinChanged()
{
    QJsonObject initPinJson;
    initPinJson["cmd"] = "pinMode";
    initPinJson["pin"] = pin;
    initPinJson["mode"] = "output";

    comPortAdapter->sendJson(initPinJson);
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
    {
        Sensor *sensor = new Sensor(value.toObject(), comPortAdapter);
        sensorList.append(sensor);
        connectSensor(sensor);
    }

    QJsonArray actuators = json["actuatorList"].toArray();
    for (const QJsonValue &value : actuators)
    {
        Actuator *actuator = new Actuator(value.toObject(), comPortAdapter);
        actuatorList.append(actuator);
        connectActuator(actuator);
    }
}


SmartItem::SmartItem(ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;
    name = "имя";
}


SmartItem::~SmartItem()
{
    for (int i = 0; i < sensorList.count(); i++)
        delete sensorList.at(i);

    for (int i = 0; i < actuatorList.count(); i++)
        delete actuatorList.at(i);
}


void SmartItem::setName(QString name)
{
    if (this->name != name)
    {
        this->name = name;
        emit paramChanged();
    }
}


void SmartItem::remove()
{
    emit removeMe(this);
    delete this;
}


void SmartItem::addSensor()
{
    Sensor *sensor = new Sensor(comPortAdapter);
    sensorList.append(sensor);
    connectSensor(sensor);

    emit sensorListChanged();
}


void SmartItem::addActuator()
{
    Actuator *actuator = new Actuator(comPortAdapter);
    actuatorList.append(actuator);
    connectActuator(actuator);

    emit actuatorListChanged();
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


void SmartItem::sensorRemoved(Sensor *sensor)
{
    sensorList.removeAll(sensor);
    emit sensorListChanged();
}


void SmartItem::actuatorRemoved(Actuator *actuator)
{
    actuatorList.removeAll(actuator);
    emit actuatorListChanged();
}


void SmartItem::connectSensor (Sensor *sensor)
{
    connect(sensor, &Sensor::paramChanged, this, &SmartItem::sensorListChanged);
    connect(sensor, &Sensor::removeMe, this, &SmartItem::sensorRemoved);
}


void SmartItem::connectActuator (Actuator *actuator)
{
    connect(actuator, &Actuator::paramChanged, this, &SmartItem::actuatorListChanged);
    connect(actuator, &Actuator::regulatorChanged, this, &SmartItem::actuatorListChanged);
    connect(actuator, &Actuator::removeMe, this, &SmartItem::actuatorRemoved);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Controller::Controller(const QJsonObject &json, ComPortAdapter *comPortAdapter)
{
    this->comPortAdapter = comPortAdapter;

    name = json["name"].toString();
    type = json["type"].toString();

    QJsonArray smartItems = json["smartItemList"].toArray();
    for (const QJsonValue &value : smartItems)
    {
        SmartItem *smartItem = new SmartItem(value.toObject(), comPortAdapter);
        smartItemList.append(smartItem);
        connectSmartItem(smartItem);
    }

    connect(this, &Controller::paramChanged, this, &Controller::saveJson);
    connect(this, &Controller::smartItemListChanged, this, &Controller::saveJson);
}


void Controller::setName(QString name)
{
    if (this->name != name)
    {
        this->name = name;
        emit paramChanged();
    }
}


void Controller::setType(QString type)
{
    if (this->type != type)
    {
        this->type = type;
        emit paramChanged();
    }
}


void Controller::addSmartItem()
{
    SmartItem *smartItem = new SmartItem(comPortAdapter);
    smartItemList.append(smartItem);
    connectSmartItem(smartItem);

    emit smartItemListChanged();
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


void Controller::smartItemRemoved(SmartItem *smartItem)
{
    smartItemList.removeAll(smartItem);
    emit smartItemListChanged();
}


void Controller::connectSmartItem(SmartItem *smartItem)
{
    connect(smartItem, &SmartItem::paramChanged, this, &Controller::saveJson);
    connect(smartItem, &SmartItem::sensorListChanged, this, &Controller::saveJson);
    connect(smartItem, &SmartItem::actuatorListChanged, this, &Controller::saveJson);
    connect(smartItem, &SmartItem::removeMe, this, &Controller::smartItemRemoved);
}
