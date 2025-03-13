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

class ComPortAdapter
{
public:
    ComPortAdapter();
    QString receptionConfig();
    void sendJson(QJsonObject json);

private slots:
    void readData();

private:
    QSerialPort serial;
};

#endif // COM_H
