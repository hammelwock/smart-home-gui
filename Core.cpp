#include "Core.h"

AllElements* allElements;

Sensor *AllElements::getSensor(QString sensorName)
{
    for (int i = 0; i < sensorList.count(); i++)
        if (sensorList.at(i)->getName() == sensorName)
            return sensorList.at(i);

    return 0;
}

void AllElements::addSensor(Sensor *sensor)
{
    sensorList.append(sensor);
    emit sensorListChanged();
}
