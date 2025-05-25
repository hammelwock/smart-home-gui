import QtQuick 2.15
import "components"

Rectangle {
    id: smartItem
    height: Math.max(220, sensorsColunn.height + 40, actuatorsColunn.height + 40)
    width: parent.width
    radius: 10
    color: "#606060"

    Row {
        id: row
        x: 20
        y: 20
        spacing: 20

        Column {
            spacing: 20

            MyTextInput {
                id: name
                anchors.horizontalCenter: parent.horizontalCenter
                text: modelData.name
                infoText: "Имя:"
                onTextChanged: modelData.name = text
            }

            Image {
                id: image
                height: 200
                width: 200
                source: "images/autoclave.webp"
            }
        }

        Column {
            id: sensorsColunn
            y: 10
            width: (smartItem.width - image.width) / 2 - 2*spacing
            spacing: 20

            ListView {
                id: sensors
                width: parent.width
                height: contentHeight
                spacing: 20

                model: modelData.sensorList

                delegate: Loader {
                    width: parent.width
                    source: modelData.type === "ds18b20" ?
                                "components/sensors/Ds18b20Sensor.qml" :
                                modelData.type === "discrete" ?
                                    "components/sensors/DiscreteSensor.qml" :
                                    "components/sensors/MySensor.qml"
                }
            }

            MyButton {
                text: "Добавить сенсор"
                mouseArea.onClicked: modelData.addSensor()
            }
        }

        Column {
            id: actuatorsColunn
            y: 10
            width: (smartItem.width - image.width) / 2 - 2*spacing
            spacing: 20

            ListView {
                id: actuators
                width: parent.width
                height: contentHeight
                y: 10
                spacing: 20
                model : modelData.actuatorList
                delegate : Actuator {}
            }

            MyButton {
                text: "Добавить актуатор"
                mouseArea.onClicked: modelData.addActuator()
            }
        }
    }

    MyCloseButton {}
}

