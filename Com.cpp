#include "Com.h"

ComPortAdapter::ComPortAdapter()
{
    serial.setPortName("ttyACM0");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    QIODevice::connect(&serial, &QSerialPort::readyRead, this, &ComPortAdapter::readData);

    if (serial.open(QIODevice::ReadWrite))
        qDebug() << "Serial port opened.";
    else
        qDebug() << "Error opening port:" << serial.errorString();
}

//QJsonObject ComPortAdapter::readConfig()
//{
//    QString config = R"(
//{
//    "home": {
//        "roomList": [
//            {
//                        "name": "комната",
//                        "smartItemList": [
//                            {
//                                "name": "Батарея",
//                                "sensorList": [
//                                    {
//                                        "name": "температура батареи",
//                                        "sensorType": "ds18b20",
//                                        "pin": 1,
//                                        "index": 1,
//                                        "unit": "°С",
//                                        "refreshRate": 1000
//                                    },
//                                    {
//                                        "name": "температура батареи",
//                                        "sensorType": "ds18b20",
//                                        "pin": 1,
//                                        "index": 2,
//                                        "unit": "°С",
//                                        "refreshRate": 1050
//                                    },
//                                    {
//                                        "name": "температура",
//                                        "sensorType": "ds18b20",
//                                        "pin": 1,
//                                        "index": 3,
//                                        "unit": "°С",
//                                        "refreshRate": 1100
//                                    },
//                                    {
//                                        "name": "температура",
//                                        "sensorType": "ds18b20",
//                                        "pin": 1,
//                                        "index": 4,
//                                        "unit": "°С",
//                                        "refreshRate": 1150
//                                    },
//                                    {
//                                        "name": "температура",
//                                        "sensorType": "ds18b20",
//                                        "pin": 1,
//                                        "index": 5,
//                                        "unit": "°С",
//                                        "refreshRate": 1170
//                                    },
//                                    {
//                                        "name": "температура",
//                                        "sensorType": "ds18b20",
//                                        "pin": 1,
//                                        "index": 6,
//                                        "unit": "°С",
//                                        "refreshRate": 1200
//                                    },
//                                    {
//                                        "name": "test",
//                                        "sensorType": "discrete",
//                                        "pin": 6,
//                                        "unit": "G",
//                                        "refreshRate": 1200
//                                    }
//                                ],
//                                "actuatorList": [
//                                    {
//                                        "name": "клапан",
//                                        "actuatorType": 0,
//                                        "pin": 13
//                                    }
//                                ]
//                            }
//                        ],
//                        "sensorList": [
//                            {
//                                "name": "температура в комнате",
//                                "sensorType": "ds18b20",
//                                "pin": 0,
//                                "index": 0,
//                                "unit": "°С",
//                                "refreshRate": 1300
//                            }
//                        ]
//            }
//        ]
//    }
//}
//)";
//    return config;
//}

void ComPortAdapter::sendJson(QJsonObject json)
{
    sendStack.append(new QJsonDocument(json));
    if (sendStack.count() == 1)
        sendJsonToCom();
}

void ComPortAdapter::readData()
{
    static QByteArray buffer;
    buffer.append(serial.readAll());

    while (buffer.contains('\n')) {
        int index = buffer.indexOf('\n');
        QByteArray line = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (!doc.isNull() && doc.isObject())
        {
            qDebug() << "Received JSON:" << doc.object();
            QJsonObject jsonObj = doc.object();
            if (jsonObj.contains("ds18b20Index"))
                emit ds18b20Read(jsonObj["ds18b20Index"].toInt(),
                    jsonObj["value"].toInt());

            if (jsonObj.contains("recd"))
                jsonRecd(&doc);
        }
        else
            qDebug() << "Invalid JSON received:" << line;
    }
}

void ComPortAdapter::sendJsonToCom()
{
    QString data = sendStack.first()->toJson(QJsonDocument::Compact);
    serial.write(data.toStdString().c_str());
    serial.flush();
    qDebug() << "Sent:" << data.toStdString().c_str();
    QTimer::singleShot(1000, this, &ComPortAdapter::sendJsonToCom);
}

void ComPortAdapter::jsonRecd(QJsonDocument *json)
{
    if (sendStack.count())
    {
        if (json == sendStack.first())
        {
            sendStack.removeFirst();
            if (sendStack.count())
                sendJsonToCom();
        }
        else
            sendJsonToCom();
    }
}


QJsonObject TestComPortAdapter::readConfig()
{
    QFile file("config1.json");
    QByteArray data;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        data = file.readAll();
        file.close();
    }

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);


    return jsonDoc.object();
}

void TestComPortAdapter::saveConfig(QJsonObject json)
{
    QJsonDocument jsonDoc(json);

    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    QFile file("config.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(jsonString.toUtf8());
        file.close();
    }
}
