import QtQuick 2.15
import "../"

Rectangle {
    id: mySensor
    height: column.height + 20
    width: parent.width
    radius: 10
    color: "#909090"

    property alias value: value.text
    property alias pin: pin
    property alias measuredQuantity: measuredQuantity

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

        Text {
            id: value
            text: "Значение: " + modelData.value + " " + modelData.measuredQuantity
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
            onTextChanged: modelData.pin = text
        }

        MyTextInput {
            id: refreshRate
            text: modelData.refreshRate
            infoText: "Частота опроса:"
            onTextChanged: modelData.refreshRate = text
        }

        MyTextInput {
            id: measuredQuantity
            text: modelData.measuredQuantity
            infoText: "Единицы:"
            onTextChanged: modelData.measuredQuantity = text
        }
    }

    MyCloseButton {}
}
