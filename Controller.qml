import QtQuick 2.15


Rectangle {
    width: parent.width
    height: Math.max(rightColumn.height, leftColumn.height) + 40
    radius: 10
    color: "#404040"

    Column {
        id: rightColumn
        x: 20
        y: 20
        spacing: 20
        Text {
            id: name
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.name
        }

        Image {
            id: controllerImg
            width: 200
            fillMode: Image.PreserveAspectFit
            source: "images/" + modelData.type + ".png"
        }
    }


    Column {
        id: leftColumn
        anchors.left: rightColumn.right
        anchors.leftMargin: 20
        y: 20
        width: parent.width
        spacing: 20

        ListView {
            id: smartItems
            width: parent.width - controllerImg.width
            height: contentHeight
            spacing: 20
            model : modelData.smartItemList
            delegate : SmartItem {}
        }
    }
}

