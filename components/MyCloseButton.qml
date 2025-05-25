import QtQuick 2.15

Rectangle {
    id: myCloseButton
    anchors.horizontalCenter: parent.right
    anchors.verticalCenter: parent.top
    width: 20
    height: 20
    radius: 20
    color: "red"

    Text {
        anchors.centerIn: parent
        text: "×"
        font.pixelSize: 20
        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: modelData.remove()
    }
}
