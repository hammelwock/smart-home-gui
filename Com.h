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
    ComPortAdapter(QString port);
    QJsonObject readConfig();
    void saveConfig(QJsonObject json);
    void sendJson(QJsonObject json);

private slots:
    void readData();

signals:
    void valueRead(int pin, double value, QString type);

private:
    void sendJsonToCom();
    void jsonRecd(QJsonDocument* json);


    QSerialPort serial;
    QList<QJsonDocument*> sendStack;
    QTimer* timer;

    QString port;
};


#endif // COM_H
