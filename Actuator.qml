import QtQuick 2.15

Rectangle {
    height: 40
    width: 250
    radius: 10
    color: "#909090"

    Row {
        x: 10
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            width: 50
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            radius: 10
            color: modelData.pin ? "#00ea13" : "#002f03"
            border.color: "black"
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: modelData.name
        }
    }
}

