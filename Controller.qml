import QtQuick 2.15
import "components"


Rectangle {
    width: parent.width
    height: Math.max(rightColumn.height, leftColumn.height) + 40
    radius: 10
    color: "#404040"

    Column {
        id: rightColumn
        x: 20
        y: 20
        width: 200
        spacing: 20

        MyTextInput {
            id: name
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.name
            infoText: "Имя:"
            onTextChanged: modelData.name = text
        }

        MyTextInput {
            id: type
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.type
            infoText: "Тип:"
            onTextChanged: modelData.type = text
        }


        Image {
            id: controllerImg
            width: parent.width
            fillMode: Image.PreserveAspectFit
            source: "images/" + modelData.type + ".png"
        }
    }


    Column {
        id: leftColumn
        anchors.left: rightColumn.right
        anchors.leftMargin: 20
        y: 20
        width: parent.width - controllerImg.width - 3*spacing
        spacing: 20

        ListView {
            id: smartItems
            width: parent.width
            height: contentHeight
            spacing: 20
            model : modelData.smartItemList
            delegate : SmartItem {}
            interactive: false
        }

        MyButton {
            text: "Добавить"
            mouseArea.onClicked: modelData.addSmartItem()
        }
    }
}

