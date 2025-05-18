import QtQuick 2.15

Rectangle {
    height: 30
    width: 150
    radius: 10
    color: "#757575"

    TextInput {
        anchors.left: parent.left
        width: parent.width - plusMinus.width
        height: parent.height
    }

    PlusMinus {
        id: plusMinus
        anchors.right: parent.right
    }
}
