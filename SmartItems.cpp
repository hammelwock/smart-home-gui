#include "SmartItems.h"
#include <QStack>
#include <QDebug>

QStringList Spliter::splitConfig(QString config)
{
    QStringList result;
    QString currentPart;
    int braceStack = 0;

    for (int i = 0; i < config.length(); ++i)
    {
        QChar c = config[i];

        if ((c == ',' || c == '{') && braceStack == 0)
        {
            result.append(currentPart);
            currentPart.clear();
        }
        else
            currentPart.append(c);

        if (c == '{')
            braceStack++;

        if (c == '}')
            braceStack--;
    }

    if (currentPart.endsWith('}'))
        currentPart.chop(1);

    result.append(currentPart);

    qDebug() << "********";
    qDebug() << result;
    qDebug() << result[0];
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sensor::Sensor(const QJsonObject &json)
{
    name = json["name"].toString();
    type = json["actuatorType"].toInt();
    pin = json["pin"].toInt();
    address = json["address"].toInt();
    measuredQuantity = json["unit"].toString();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Actuator::Actuator(const QJsonObject &json)
{
    name = json["name"].toString();
    type = json["actuatorType"].toInt();
    pin = json["pin"].toInt();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SmartItem::SmartItem(const QJsonObject &json)
{
    name = json["name"].toString();

    QJsonArray sensors = json["sensorList"].toArray();
    for (const QJsonValue &value : sensors)
        sensorList.append(new Sensor(value.toObject()));

    QJsonArray actuators = json["actuatorList"].toArray();
    for (const QJsonValue &value : actuators)
        actuatorList.append(new Actuator(value.toObject()));
}

Room::Room(const QJsonObject &json)
{
    name = json["name"].toString();

    QJsonArray smartItems = json["smartItemList"].toArray();
    for (const QJsonValue &value : smartItems)
        smartItemList.append(new SmartItem(value.toObject()));

    QJsonArray sensors = json["sensorList"].toArray();
    for (const QJsonValue &value : sensors)
        sensorList.append(new Sensor(value.toObject()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Home::Home(QString config)
{
    {
        QJsonDocument doc = QJsonDocument::fromJson(config.toUtf8());
        QJsonObject root = doc.object();

        QJsonObject home = root["home"].toObject();

        QJsonArray rooms = home["roomList"].toArray();
        qDebug() << "rooms содержит:" << rooms;
        for (const QJsonValue &value : rooms)
            roomList.append(new Room(value.toObject()));

        QJsonArray sensors = home["sensorList"].toArray();
        for (const QJsonValue &value : sensors)
            sensorList.append(new Sensor(value.toObject()));
    }
}


