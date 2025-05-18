#ifndef SMARTITEMS_H
#define SMARTITEMS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "Com.h"

class Sensor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName() NOTIFY nameChanged())
    Q_PROPERTY(QString type READ getType() NOTIFY typeChanged())
    Q_PROPERTY(int pin READ getPin() NOTIFY pinChanged())
    Q_PROPERTY(int refreshRate READ getRefreshRate() NOTIFY refreshRateChanged())
    Q_PROPERTY(QString measuredQuantity READ readMeasuredQuantity() NOTIFY measuredQuantityChanged())
    Q_PROPERTY(double value READ getValue() WRITE setValue() NOTIFY valueChanged())

public:
    Sensor(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString getName(){return name;}
    QString getType(){return type;}
    int getPin(){return pin;}
    int getRefreshRate(){return refreshRate;}
    double getValue(){return value;}
    void setValue(double value){this->value = value; valueChanged();}

    QString readMeasuredQuantity(){return measuredQuantity;}

    QJsonObject saveJson();

signals:
    void nameChanged();
    void typeChanged();
    void pinChanged();
    void refreshRateChanged();
    void measuredQuantityChanged();
    void valueChanged();

private slots:
    void refreshValue();
    void readValue(int pin, double value, QString type);

private:
    QString name;
    QString type;
    int pin;
    int refreshRate;
    QString measuredQuantity;
    double value;
    ComPortAdapter *comPortAdapter;
};


class Regulator : public QObject
{
    Q_OBJECT
public:
    Regulator(const QJsonObject &json);

    QJsonObject saveJson();

signals:
    void adjusted(bool value);

private slots:
    void regulate();
    void setSensor();

private:
    int target;
    int hysteresis;
    bool state;
    bool invert;
    QString sensorName;
    Sensor* sensor;
};


class Actuator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName() NOTIFY nameChanged())
    Q_PROPERTY(int type READ getType() NOTIFY typeChanged())
    Q_PROPERTY(int pin READ getPin() NOTIFY pinChanged())
    Q_PROPERTY(int value READ getValue() WRITE setValue() NOTIFY valueChanged())

public:
    Actuator(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString getName(){return name;}
    int getType(){return type;}
    int getPin(){return pin;}
    int getValue(){return value;}
    void setValue(int value);

    QJsonObject saveJson();

signals:
    void nameChanged();
    void typeChanged();
    void pinChanged();
    void valueChanged();

private:
    void sendValue();

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


class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName() NOTIFY nameChanged())
    Q_PROPERTY(QString type READ getType() NOTIFY typeChanged())
    Q_PROPERTY(QList<QObject*> smartItemList READ getSmartItemList() NOTIFY smartItemListChanged())

public:
    Controller(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString getName(){return name;}
    QString getType(){return type;}
    QList<QObject*> getSmartItemList(){return smartItemList;}

    QJsonObject saveJson();

signals:
    void nameChanged();
    void typeChanged();
    void smartItemListChanged();

private:
    QString name;
    QString type;
    QList<QObject*> smartItemList;
    ComPortAdapter *comPortAdapter;
};


class Home : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> controllerList READ getControllerList() NOTIFY controllerListChanged())

public:
    Home(){}
    QList<QObject*> getControllerList(){return controllerList;}
    void addController(Controller* controller){controllerList.append(controller);}

signals:
    void controllerListChanged();

private:
    QList<QObject*> controllerList;
};


#endif // SMARTITEMS_H
