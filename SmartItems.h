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
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY paramChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY paramChanged)
    Q_PROPERTY(int pin READ getPin WRITE setPin NOTIFY paramChanged)
    Q_PROPERTY(int refreshRate READ getRefreshRate WRITE setRefreshRate NOTIFY paramChanged)
    Q_PROPERTY(QString measuredQuantity READ getMeasuredQuantity WRITE setMeasuredQuantity NOTIFY paramChanged)
    Q_PROPERTY(double value READ getValue() WRITE setValue() NOTIFY valueChanged())

public:
    Sensor(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    Sensor(ComPortAdapter *comPortAdapter);

    QString getName(){return name;}
    void setName(QString name);
    QString getType(){return type;}
    void setType(QString type);
    int getPin(){return pin;}
    void setPin(int pin);
    int getRefreshRate(){return refreshRate;}
    void setRefreshRate(int refreshRate);
    QString getMeasuredQuantity(){return measuredQuantity;}
    void setMeasuredQuantity(QString measuredQuantity);

    double getValue(){return value;}
    void setValue(double value){this->value = value; valueChanged();}

    Q_INVOKABLE void remove();
    QJsonObject saveJson();

signals:
    void paramChanged();
    void valueChanged();
    void removeMe(Sensor *sensor);

private slots:
    void onPinChanged();
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


class Regulator : public QObject {
    Q_OBJECT
    Q_PROPERTY(int target READ getTarget WRITE setTarget NOTIFY paramChanged)
    Q_PROPERTY(double kp READ getKp WRITE setKp NOTIFY paramChanged)
    Q_PROPERTY(double ki READ getKi WRITE setKi NOTIFY paramChanged)
    Q_PROPERTY(double kd READ getKd WRITE setKd NOTIFY paramChanged)
    Q_PROPERTY(bool invert READ getInvert WRITE setInvert NOTIFY paramChanged)
    Q_PROPERTY(QString sensorName READ getSensorName WRITE setSensorName NOTIFY paramChanged)

public:
    Regulator();
    Regulator(const QJsonObject &json);

    int getTarget() const { return target; }
    void setTarget(int t);
    double getKp() const { return kp; }
    void setKp(double v);
    double getKi() const { return ki; }
    void setKi(double v);
    double getKd() const { return kd; }
    void setKd(double v);
    bool getInvert() const { return invert; }
    void setInvert(bool i);
    QString getSensorName() const { return sensorName; }
    void setSensorName(const QString &name);

    QJsonObject saveJson();

signals:
    void paramChanged();
    void adjusted(double value);

private slots:
    void regulate();
    void setSensor();

private:
    int target;
    double kp, ki, kd;
    bool invert;
    double integral;
    double prevError;
    QElapsedTimer timer;
    QString sensorName;
    Sensor* sensor = nullptr;
    QMetaObject::Connection sensorConnection;
};


class Actuator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY paramChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY paramChanged)
    Q_PROPERTY(int pin READ getPin WRITE setPin NOTIFY paramChanged)
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QObject* regulator READ getRegulator NOTIFY regulatorChanged)

public:
    Actuator(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    Actuator(ComPortAdapter *comPortAdapter);
    ~Actuator();

    QString getName(){return name;}
    void setName(QString name);
    QString getType(){return type;}
    void setType(QString type);
    int getPin(){return pin;}
    void setPin(int pin);
    int getValue(){return value;}
    void setValue(int value);
    QObject* getRegulator(){return regulator;}

    Q_INVOKABLE void remove();
    Q_INVOKABLE void addRegulator();
    QJsonObject saveJson();

signals:
    void paramChanged();
    void valueChanged();
    void regulatorChanged();
    void removeMe(Actuator *actuator);

private slots:
    void onPinChanged();

private:
    void sendValue();

    QString name;
    QString type;
    int pin;
    int value;
    ComPortAdapter *comPortAdapter;
    Regulator* regulator;
};


class SmartItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY paramChanged)
    Q_PROPERTY(QList<QObject*> sensorList READ getSensorList NOTIFY sensorListChanged)
    Q_PROPERTY(QList<QObject*> actuatorList READ getActuatorList NOTIFY actuatorListChanged)
public:
    SmartItem(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    SmartItem(ComPortAdapter *comPortAdapter);
    ~SmartItem();

    QString getName(){return name;}
    void setName(QString name);
    QList<QObject*> getSensorList(){return sensorList;}
    QList<QObject*> getActuatorList(){return actuatorList;}

    Q_INVOKABLE void remove();
    Q_INVOKABLE void addSensor();
    Q_INVOKABLE void addActuator();
    QJsonObject saveJson();

signals:
    void paramChanged();
    void sensorListChanged();
    void actuatorListChanged();
    void removeMe(SmartItem *smartItem);

private slots:
    void sensorRemoved(Sensor *sensor);
    void actuatorRemoved(Actuator *actuator);

private:
    void connectSensor(Sensor *sensor);
    void connectActuator(Actuator *actuator);

    QString name;
    QList<QObject*> sensorList;
    QList<QObject*> actuatorList;
    ComPortAdapter *comPortAdapter;
};


class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY paramChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY paramChanged)
    Q_PROPERTY(QList<QObject*> smartItemList READ getSmartItemList NOTIFY smartItemListChanged)

public:
    Controller(const QJsonObject &json, ComPortAdapter *comPortAdapter);
    QString getName(){return name;}
    void setName(QString name);
    QString getType(){return type;}
    void setType(QString type);
    QList<QObject*> getSmartItemList(){return smartItemList;}

    Q_INVOKABLE void addSmartItem();
    QJsonObject saveJson();

signals:
    void paramChanged();
    void smartItemListChanged();

private slots:
    void smartItemRemoved(SmartItem *smartItem);

private:
    void connectSmartItem(SmartItem *smartItem);

    QString name;
    QString type;
    QList<QObject*> smartItemList;
    ComPortAdapter *comPortAdapter;
};


class Home : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> controllerList READ getControllerList NOTIFY controllerListChanged)

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
