import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    height: 150
    width: 250
    radius: 10
    color: "#808080"
    Column {
        anchors.fill: parent
        spacing: 10

        Item {
            width: parent.width
            height: 40

            Row{
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - 100
                anchors.left: parent.left
                Text {
                    text: "Цель: "
                }

                IntInput {

                }
            }
        }

        Item {
            width: parent.width
            height: 40

            Row{
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - 100
                anchors.left: parent.left
                Text {
                    text: "Гистерезис: "
                }
                IntInput {

                }

            }
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
                checked: false
            }
        }
    }
}

