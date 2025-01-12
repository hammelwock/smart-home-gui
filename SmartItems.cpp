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

Sensor::Sensor(QString config)
{
    QStringList configList = splitConfig(config);
    if (configList.count() > 2)
    {
        name = configList[0];
        type = configList[1].toInt();
        pin = configList[2].toInt();
        address = configList[3].toInt();
        measuredQuantity = configList[4];
    }
    else
    {
        name = "error";
        type = -1;
        pin = -1;
        address = -1;
        measuredQuantity = "";
    } 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Actuator::Actuator(QString config)
{
    QStringList configList = splitConfig(config);
    if (configList.count() > 2)
    {
        name = configList[0];
        type = configList[1].toInt();
        pin = configList[2].toInt();
    }
    else
    {
        name = "error";
        type = -1;
        pin = -1;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SmartItem::SmartItem(QString config)
{
    QStringList configList = splitConfig(config);

    name = configList[0];

    for (int i = 1; i < configList.count(); i += 2)
    {
        switch (configList[i].toInt())
        {
        case ObjectType::ACTUATOR:
            actuatorList.append(new class Actuator(configList[i + 1]));
            break;
        case ObjectType::SENSOR:
            sensorList.append(new class Sensor(configList[i + 1]));
            break;
        }
    }
}

Room::Room(QString config)
{
    {
        QStringList configList = splitConfig(config);

        name = configList[0];

        for (int i = 1; i < configList.count(); i += 2)
        {
            switch (configList[i].toInt())
            {
            case ObjectType::SMARTITEM:
                smartItemList.append(new class SmartItem(configList[i + 1]));
                break;
            case ObjectType::SENSOR:
                sensorList.append(new class Sensor(configList[i + 1]));
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Home::Home(QString config)
{
    {
        QStringList configList = splitConfig(config);

        for (int i = 0; i < configList.count(); i += 2)
        {
            switch (configList[i].toInt())
            {
            case ObjectType::ROOM:
                roomList.append(new class Room(configList[i + 1]));
                break;
            case ObjectType::SENSOR:
                sensorList.append(new class Sensor(configList[i + 1]));
                break;
            }
        }
    }
}


