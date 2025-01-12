#ifndef SMARTITEMS_H
#define SMARTITEMS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QAbstractListModel>


class Spliter
{
protected:
    QStringList splitConfig(QString config);
};


class Sensor : public QObject, protected Spliter
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(int type READ readType() NOTIFY typeChanged())
    Q_PROPERTY(int pin READ readPin() NOTIFY pinChanged())
    Q_PROPERTY(int address READ readAddress() NOTIFY addressChanged())
    Q_PROPERTY(QString measuredQuantity READ readMeasuredQuantity() NOTIFY measuredQuantityChanged())

public:
    Sensor(QString config);
    QString readName(){return name;}
    int readType(){return type;}
    int readPin(){return pin;}
    int readAddress(){return address;}
    QString readMeasuredQuantity(){return measuredQuantity;}

signals:
    void nameChanged();
    void typeChanged();
    void pinChanged();
    void addressChanged();
    void measuredQuantityChanged();

private:
    QString name;
    int type;
    int pin;
    int address;
    QString measuredQuantity;
};


class Actuator : public QObject, protected Spliter
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(int type READ readType() NOTIFY typeChanged())
    Q_PROPERTY(int pin READ readPin() NOTIFY pinChanged())

public:
    Actuator(QString config);
    QString readName(){return name;}
    int readType(){return type;}
    int readPin(){return pin;}

signals:
    void nameChanged();
    void typeChanged();
    void pinChanged();

private:
    QString name;
    int type;
    int pin;
};


class SmartItem : public QObject, protected Spliter
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())
    Q_PROPERTY(QList<QObject*> actuatorList READ readActuatorList() NOTIFY actuatorListChanged())

public:
    SmartItem(QString config);
    QString readName(){return name;}
    QList<QObject*> readSensorList(){return sensorList;}
    QList<QObject*> readActuatorList(){return actuatorList;}

signals:
    void nameChanged();
    void sensorListChanged();
    void actuatorListChanged();

private:
    QString name;
    QList<QObject*> sensorList;
    QList<QObject*> actuatorList;
};


class Room : public QObject, protected Spliter
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName() NOTIFY nameChanged())
    Q_PROPERTY(QList<QObject*> smartItemList READ readSmartItemList() NOTIFY smartItemListChanged())
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())

public:
    Room(QString config);
    QString readName(){return name;}
    QList<QObject*> readSmartItemList(){return smartItemList;}
    QList<QObject*> readSensorList(){return sensorList;}

signals:
    void nameChanged();
    void smartItemListChanged();
    void sensorListChanged();

private:
    QString name;
    QList<QObject*> smartItemList;
    QList<QObject*> sensorList;
};


class Home : public QObject, protected Spliter
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> roomList READ readRoomList() NOTIFY roomListChanged())
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())

public:
    Home(QString config);
    QList<QObject*> readRoomList(){return roomList;}
    QList<QObject*> readSensorList(){return sensorList;}

signals:
    void roomListChanged();
    void sensorListChanged();

private:
    QList<QObject*> roomList;
    QList<QObject*> sensorList;
};


enum ObjectType
{
    HOME = 0,
    ROOM = 1,
    SMARTITEM = 2,
    SENSOR = 3,
    ACTUATOR = 4,
};

#endif // SMARTITEMS_H
