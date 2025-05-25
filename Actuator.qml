import QtQuick 2.15
import "components"


Rectangle {
    width: parent.width
    height: column.height + 20
    radius: 10
    color: "#909090"

    Column {
        id: column
        x: 20
        y: 10
        width: parent.width
        spacing: 10

        MyTextInput {
            id: name
            text: modelData.name
            infoText: "Имя:"
            onTextChanged: modelData.name = text
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
                text: "Значение: " + modelData.value
            }
        }

        MyTextInput {
            id: type
            text: modelData.type
            infoText: "Тип:"
            onTextChanged: modelData.type = text
        }

        MyTextInput {
            id: pin
            text: modelData.pin
            infoText: "Пин:"
            onTextChanged: modelData.зшт = text
        }

        MyButton {
            text: "Добавить регудятор"
            mouseArea.onClicked: modelData.addRegulator()
            width: modelData.regulator ? 0 : parent.width - 40
            visible: !modelData.regulator
        }

        Loader {
            source: modelData.regulator ? "Regulator.qml" : ""
        }
    }

    MyCloseButton {}
}

