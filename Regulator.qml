import QtQuick 2.15
import QtQuick.Controls 2.15
import "components"

Rectangle {
    height: 200
    width: 300
    radius: 10
    color: "#808080"

    Column {
        id: regolumn
        x: 20
        y: 10
        anchors.fill: parent
        width: parent.width
        spacing: 10

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.target
            infoText: "Цель:"
            onTextChanged: modelData.target = text
        }

        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.hysteresis
            infoText: "Гистерезис:"
            onTextChanged: modelData.hysteresis = text
        }


        MyTextInput {
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.sensorName
            infoText: "Сенсор:"
            onTextChanged: modelData.sensorName = text
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
                checked: modelData.invert
            }
        }
    } 

    MyCloseButton {}
}

