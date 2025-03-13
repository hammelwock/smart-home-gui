#include "Com.h"

ComPortAdapter::ComPortAdapter()
{
    serial.setPortName();
    serial.setBaudRate(QSerialPort::Baud9600);
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

QString ComPortAdapter::receptionConfig()
{
    QString config = R"(
{
    "home": {
        "roomList": [
            {
                        "type": "room",
                        "name": "комната",
                        "smartItemList": [
                            {
                                "type": "smartItem",
                                "name": "Батарея",
                                "sensorList": [
                                    {
                                        "type": "sensor",
                                        "name": "температура батареи",
                                        "sensorType": 0,
                                        "pin": 1,
                                        "address": 372,
                                        "unit": "°С"
                                    }
                                ],
                                "actuatorList": [
                                    {
                                        "type": "actuator",
                                        "name": "клапан",
                                        "actuatorType": 0,
                                        "pin": 1
                                    }
                                ]
                            }
                        ],
                        "sensorList": [
                            {
                                "type": "sensor",
                                "name": "температура в комнате",
                                "sensorType": 0,
                                "pin": 1,
                                "address": 372,
                                "unit": "°С"
                            }
                        ]
            }
        ]
    }
}
)";
    return config;
}

void ComPortAdapter::sendJson(QJsonObject json)
{
    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + '\n';
    serial.write(data);
    qDebug() << "Sent:" << data;
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
        if (!doc.isNull() && doc.isObject()) {
            qDebug() << "Received JSON:" << doc.object();
        } else {
            qDebug() << "Invalid JSON received:" << line;
        }
    }
}

