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
            model : modelData.sensorList
            delegate : Sensor {}
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

        Column {
            y: 10
            spacing: 20
            Rectangle {
                height: 100
                width: 170
                radius: 10
                color: "#909090"

                Text {
                    x: 20
                    y: 15
                    width: 100
                    text: "Заданная температура"
                    wrapMode: Text.Wrap
                }

                Rectangle {
                    x: 120
                    y: 0
                    height: 50
                    width: 50
                    radius: 10
                    border.color: "black"
                    color: "#909090"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "+"
                    }
                }

                Rectangle {
                    x: 10
                    y: 60
                    height: 30
                    width: 100
                    color: "#606060"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "200 °С"
                    }
                }

                Rectangle {
                    x: 120
                    y: 50
                    height: 50
                    width: 50
                    radius: 10
                    border.color: "black"
                    color: "#909090"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "-"
                    }
                }
            }

            Rectangle {
                height: 100
                width: 170
                radius: 10
                color: "#909090"

                Text {
                    x: 20
                    y: 15
                    width: 100
                    text: "Заданное давление"
                    wrapMode: Text.Wrap
                }

                Rectangle {
                    x: 120
                    y: 0
                    height: 50
                    width: 50
                    radius: 10
                    border.color: "black"
                    color: "#909090"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "+"
                    }
                }

                Rectangle {
                    x: 10
                    y: 60
                    height: 30
                    width: 100
                    color: "#606060"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "400 кПа"
                    }
                }

                Rectangle {
                    x: 120
                    y: 50
                    height: 50
                    width: 50
                    radius: 10
                    border.color: "black"
                    color: "#909090"
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "-"
                    }
                }
            }
            Rectangle {
                height: 100
                width: 100
                radius: 50
                border.color: "black"
                color: "#ff0000"
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Stop!"
                }
            }
        }

    }
}

