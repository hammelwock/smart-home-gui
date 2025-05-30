import QtQuick 2.15

MySensor {


    Rectangle {
        width: 20
        height: parent.height - 40
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        radius: 10
        color: "#002f03"
        border.color: "black"

        Rectangle {
            width: parent.width - 4
            height: parent.height - 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 10
            color: modelData.value ? "#00ea13" : "#002f03"
        }
    }
}
