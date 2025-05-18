import QtQuick 2.15

Rectangle {
    height: 40
    width: 200
    radius: 10
    color: "#909090"
    Text {
        x: 20
        anchors.verticalCenter: parent.verticalCenter
        text: modelData.name + ": " + modelData.value
    }
}
