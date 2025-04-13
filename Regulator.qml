import QtQuick 2.15

Rectangle {
    height: 150
    width: 250
    radius: 10
    color: "#909000"
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

                Text {
                    text: "400 кПа"
                }
            }

            PlusMinus {
                width: 100
                anchors.right: parent.right
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

                Text {
                    text: "400 кПа"
                }
            }

            PlusMinus {
                width: 100
                anchors.right: parent.right
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

            PlusMinus {
                width: 100
                anchors.right: parent.right
            }
        }
    }
}

