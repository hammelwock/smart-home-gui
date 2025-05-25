import QtQuick 2.15

Item {
    id: myTextInput
    width: parent.width
    height: 40

    property alias text: textInput.text
    property alias infoText: infoText.text

    Text {
        anchors.verticalCenter: parent.verticalCenter
        id: infoText
    }

    Rectangle {
        anchors.left: infoText.right
        anchors.leftMargin: 10
        width: Math.min(Math.max(textInput.contentWidth + 20, 75),
                        parent.width - infoText.contentWidth - 20)
        height: 40
        color: "skyblue"
        radius: 10

        TextInput {
            id: textInput
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
