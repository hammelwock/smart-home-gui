import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 1280
    height: 720
    visible: true
    color: "#303030"

    ListView {
        width: parent.width - 300
        height: parent.height
        x: 300
        spacing: 20
        model: home.roomList
        delegate: Room{}
    }
}
