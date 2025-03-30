#ifndef COM_H
#define COM_H

#include <QString>
#include <QCoreApplication>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class ComPortAdapter : public QObject
{
    Q_OBJECT
public:
    ComPortAdapter();
    QString receptionConfig();
    void sendJson(QJsonObject json);

private slots:
    void readData();

signals:
    void ds18b20Read(int index, double value);

private:
    void sendJsonToCom();
    void jsonRecd(QJsonDocument* json);

    QSerialPort serial;
    QList<QJsonDocument*> sendStack;
};

#endif // COM_H
