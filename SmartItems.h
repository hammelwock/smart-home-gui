#ifndef SMARTITEMS_H
#define SMARTITEMS_H

#include "Com.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class Sensor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName() NOTIFY nameChanged())
    Q_PROPERTY(QString type READ getType() NOTIFY typeChanged())
    Q_PROPERTY(int pin READ getPin() NOTIFY pinChanged())
    Q_PROPERTY(int index READ getIndex() NOTIFY indexChanged())
    Q_PROPERTY(int refreshRate READ getRefreshRate() NOTIFY refreshRateChanged())
    Q_PROPERTY(QString measuredQuantity READ readMeasuredQuantity() NOTIFY measuredQuantityChanged())
    Q_PROPERTY(double value READ getValue() WRITE setValue() NOTIFY valueChanged())

public:
    Sensor(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString getName(){return name;}
    QString getType(){return type;}
    int getPin(){return pin;}
    int getIndex(){return index;}
    int getRefreshRate(){return refreshRate;}
    double getValue(){return value;}
    void setValue(double value){this->value = value; valueChanged();}

    QString readMeasuredQuantity(){return measuredQuantity;}

    QJsonObject saveJson();

signals:
    void nameChanged();
    void typeChanged();
    void pinChanged();
    void indexChanged();
    void refreshRateChanged();
    void measuredQuantityChanged();
    void valueChanged();

private slots:
    void refreshValue();
    void readValue(int index, double value);

private:
    QString name;
    QString type;
    int pin;
    int index;
    int refreshRate;
    QString measuredQuantity;
    double value;
    ComPortAdapter *comPortAdapter;
};


class Regulator : public QObject
{
    Q_OBJECT
public:
    Regulator(int target, int hysteresis, bool invert);
    bool regulate();

    QJsonObject saveJson();

private:
    int target;
    int hysteresis;
    bool state;
    bool invert;
    Sensor* sensor;
};


class Actuator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(int type READ readType() NOTIFY typeChanged())
    Q_PROPERTY(int pin READ readPin() NOTIFY pinChanged())
    Q_PROPERTY(int value READ getValue() WRITE setValue() NOTIFY valueChanged())

public:
    Actuator(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString readName(){return name;}
    int readType(){return type;}
    int readPin(){return pin;}
    int getValue(){return value;}
    void setValue(int value){this->value = value; valueChanged();}

    QJsonObject saveJson();

signals:
    void nameChanged();
    void typeChanged();
    void pinChanged();
    void valueChanged();

private:
    void sendLevl();

    QString name;
    int type;
    int pin;
    int value;
    ComPortAdapter *comPortAdapter;
    Regulator* regulator;
};


class SmartItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())
    Q_PROPERTY(QList<QObject*> actuatorList READ readActuatorList() NOTIFY actuatorListChanged())

public:
    SmartItem(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString readName(){return name;}
    QList<QObject*> readSensorList(){return sensorList;}
    QList<QObject*> readActuatorList(){return actuatorList;}

    QJsonObject saveJson();

signals:
    void nameChanged();
    void sensorListChanged();
    void actuatorListChanged();

private:
    QString name;
    QList<QObject*> sensorList;
    QList<QObject*> actuatorList;
    ComPortAdapter *comPortAdapter;
};


class Room : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(QList<QObject*> smartItemList READ readSmartItemList() NOTIFY smartItemListChanged())
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())

public:
    Room(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString readName(){return name;}
    QList<QObject*> readSmartItemList(){return smartItemList;}
    QList<QObject*> readSensorList(){return sensorList;}

    QJsonObject saveJson();

signals:
    void nameChanged();
    void smartItemListChanged();
    void sensorListChanged();

private:
    QString name;
    QList<QObject*> smartItemList;
    QList<QObject*> sensorList;
    ComPortAdapter *comPortAdapter;
};


class Home : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> roomList READ readRoomList() NOTIFY roomListChanged())
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())

public:
    Home(QString config, ComPortAdapter *comPortAdapter);
    QList<QObject*> readRoomList(){return roomList;}
    QList<QObject*> readSensorList(){return sensorList;}

    Q_INVOKABLE void saveJson();

signals:
    void roomListChanged();
    void sensorListChanged();

private:
    QList<QObject*> roomList;
    QList<QObject*> sensorList;
    ComPortAdapter *comPortAdapter;
};


#endif // SMARTITEMS_H
