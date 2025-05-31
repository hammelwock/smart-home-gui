#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "SmartItems.h"

class AllElements : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())

public:
    AllElements(){}
    Sensor* getSensor(QString sensorName);
    void addSensor(Sensor* sensor);

signals:
    void sensorListChanged();

private:
//    QList<QObject*> roomList;
//    QList<QObject*> smartItemList;
//    QList<QObject*> actuatorList;
    QList<Sensor*> sensorList;
};

extern AllElements* allElements;
extern Home* home;

#endif // CORE_H
