import QtQuick 2.15

Rectangle {
    height: Math.max(220, sensors.height + 40, actuators.height + 40)
    width: parent.width - 40
    radius: 10
    color: "#606060"

    Row {
        id: row
        x: 20
        y: 20
        spacing: 20
        SignalLamp {}

        Column {
            spacing: 20
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: modelData.name
            }

            Image {
                height: 150
                width: 150
                source: "images/autoclave.webp"
            }
        }


        ListView {
            id: sensors
            width: 200
            height: contentHeight
            y: 10
            spacing: 20

            model: modelData.sensorList

            delegate: Loader {
                source: modelData.type === "ds18b20" ? "sensors/Ds18b20Sensor.qml" :
                        modelData.type === "discrete" ? "sensors/DiscreteSensor.qml" :
                        "sensors/DiscreteSensor.qml"
            }
        }

        ListView {
            id: actuators
            width: 250
            height: contentHeight
            y: 10
            spacing: 20
            model : modelData.actuatorList
            delegate : Actuator {}
        }
    }
}

