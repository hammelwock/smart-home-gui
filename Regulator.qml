import QtQuick 2.15
import QtQuick.Controls 2.15
import "components"

Rectangle {
    height: 300
    width: 300
    radius: 10
    color: "#808080"

    Column {
        id: regColumn
        x: 20
        y: 10
        anchors.fill: parent
        width: parent.width
        spacing: 10

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.regulator.target
            infoText: "Цель:"
            onTextChanged: modelData.regulator.target = text
        }

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.regulator.kp
            infoText: "П:"
            onTextChanged: modelData.regulator.kp = text
        }

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.regulator.ki
            infoText: "И:"
            onTextChanged: modelData.regulator.ki = text
        }

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.regulator.kd
            infoText: "Д:"
            onTextChanged: modelData.regulator.kd = text
        }

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.regulator.sensorName
            infoText: "Сенсор:"
            onTextChanged: modelData.regulator.sensorName = text
        }

        Item {
            width: parent.width
            height: 40

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                text: "Инвертировать: "
            }

            Switch {
                id: toggle
                anchors.right: parent.right
                checked: modelData.regulator.invert
                onCheckedChanged: modelData.regulator.invert = checked
            }
        }
    } 

    MyCloseButton {}
}

