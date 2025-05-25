import QtQuick 2.15

Rectangle {
    id: myButton
    width: parent.width
    height: 60
    radius: 10
    color: "royalblue"

    property alias text: text.text
    property alias mouseArea: mouseArea

    Text {
        id: text
        anchors.centerIn: parent
        text: "Добавить актуатор"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
}
