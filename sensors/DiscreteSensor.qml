import QtQuick 2.15

Rectangle {
    height: 40
    width: 200
    radius: 10
    color: "#909090"
    Text {
        x: 20
        anchors.verticalCenter: parent.verticalCenter
        text: modelData.name + ": " + modelData.value + modelData.measuredQuantity
    }

    Rectangle {
        width: 10
        height: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        radius: 10
        color: modelData.value ? "#00ea13" : "#002f03"
        border.color: "black"
    }
}

