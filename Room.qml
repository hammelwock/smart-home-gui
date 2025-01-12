import QtQuick 2.15


Rectangle {
    width: parent.width
    height: column.height + 40
    radius: 10
    color: "#404040"

    Column {
        id: column
        x: 20
        y: 20
        width: parent.width
        spacing: 20

        Text {
            id: name
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData.name
        }

        ListView {
            id: smartItems
            width: parent.width
            height: contentHeight
            spacing: 20
            model : modelData.smartItemList
            delegate : SmartItem {}
        }

        ListView {
            id: sensors
            width: parent.width
            height: contentHeight
            spacing: 20
            model : modelData.sensorList
            delegate : Sensor {
                width: 400
                color: "#606060"
            }
        }
    }
}

