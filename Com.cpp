#include "Com.h"
#include "Core.h"

ComPortAdapter::ComPortAdapter(QString port)
{
    this->port = port;
    serial.setPortName(port);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    QIODevice::connect(&serial, &QSerialPort::readyRead, this, &ComPortAdapter::readData);

    if (serial.open(QIODevice::ReadWrite))
    {
        qDebug() << "Serial port opened.";

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ComPortAdapter::sendJsonToCom);
        timer->start(1000);
        home->addController(new Controller(readConfig(), this));
    }
    else
        qDebug() << "Error opening port:" << serial.errorString();
}


void ComPortAdapter::sendJson(QJsonObject json)
{
    sendStack.append(new QJsonDocument(json));
    if(sendStack.count() == 1)
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

            if (jsonObj.contains("value"))
                emit valueRead(jsonObj["pin"].toInt(),
                    jsonObj["value"].toDouble(),
                    jsonObj["type"].toString());

            if (jsonObj.contains("recd"))
                jsonRecd(&doc);
        }
        else
        {
            qDebug() << "Invalid JSON received:" << line;
            timer->start(100);
        }
    }
}

void ComPortAdapter::sendJsonToCom()
{
    if (sendStack.count())
    {
        QString data = sendStack.first()->toJson(QJsonDocument::Compact);
        serial.write(data.toStdString().c_str());
        serial.flush();
        qDebug() << "Sent:" << data.toStdString().c_str();
    }
}

void ComPortAdapter::jsonRecd(QJsonDocument *json)
{
    if (sendStack.count())
    {
        QJsonObject obj = json->object();

        obj.remove("recd");
        obj.remove("value");

        if (obj == sendStack.first()->object())
        {
            sendStack.removeFirst();
            timer->stop();

            qDebug() << "!!!!!!!!!!!! Принято, в очереди " << sendStack.count() << " !!!!!!!!!!!!";
            if(sendStack.count())
                sendJsonToCom();
        }
        else
            timer->start(100);
    }
}


QJsonObject ComPortAdapter::readConfig()
{
    QFile file("config_" + port + ".json");
    QByteArray data;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        data = file.readAll();
        file.close();
    }

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);


    return jsonDoc.object();
}

void ComPortAdapter::saveConfig(QJsonObject config)
{
//    QJsonObject obj;
//    obj["cmd"] = "saveConfig";
//    obj["config"] = config;

//    sendJson(obj);

    QJsonDocument jsonDoc(config);

    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    QFile file("config_" + port + ".json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(jsonString.toUtf8());
        file.close();
    }
}
