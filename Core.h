#ifndef CORE_H
#define CORE_H

#include <QObject>

class AllElements : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> sensorList READ readSensorList() NOTIFY sensorListChanged())

public:
    AllElements(){}
    QList<QObject*> readSensorList(){return sensorList;}
    void addSensor(QObject* sensor){sensorList.append(sensor);}

signals:
    void sensorListChanged();

private:
    QList<QObject*> roomList;
    QList<QObject*> smartItemList;
    QList<QObject*> actuatorList;
    QList<QObject*> sensorList;
};

extern AllElements* allElements;

#endif // CORE_H
