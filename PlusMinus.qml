import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: 60
    height: 30

    signal increment()
    signal decrement()

    Row {
        anchors.fill: parent
        anchors.margins: 0

        Rectangle {
            height: 30
            width: 30
            radius: 10
            border.color: "black"
            color: "#909090"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "+"
            }

            MouseArea {
                anchors.fill: parent
                onClicked:
                    increment()
            }
        }

        Rectangle {
            height: 30
            width: 30
            radius: 10
            border.color: "black"
            color: "#909090"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "-"
            }

            MouseArea {
                anchors.fill: parent
                onClicked:
                    decrement()
            }
        }
    }
}
