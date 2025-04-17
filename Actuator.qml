import QtQuick 2.15

Rectangle {
    width: 250
    height: 200
    radius: 10
    color: "#909090"

    Column {
        anchors.fill: parent
        spacing: 10

        Regulator {

        }

        Row {
            x: 10
            spacing: 10
            Rectangle {
                width: 50
                height: 20
                anchors.verticalCenter: parent.verticalCenter
                radius: 10
                color: modelData.value ? "#00ea13" : "#002f03"
                border.color: "black"
                MouseArea {
                    anchors.fill: parent
                    onClicked:
                        modelData.value = !modelData.value
                }
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: modelData.name
            }
        }
    }
}

