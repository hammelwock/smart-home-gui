import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property alias checked: checkbox.checked
    property alias value: slider.value
    property alias minimumValue: slider.from
    property alias maximumValue: slider.to
    property alias stepSize: slider.stepSize
    property string label: "Option"

    signal toggled(bool checked)


    Row {
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter

        CheckBox {
            id: checkbox
            text: root.label
            checked: true
            onCheckedChanged: root.toggled(checked)
        }

        Slider {
            id: slider
            enabled: checkbox.checked
            from: 0
            to: 100
            stepSize: 1
            value: 50
            width: 200

            onValueChanged: root.valueChanged(value)
        }

        Text {
            text: Math.round(slider.value).toString()
            width: 40
            horizontalAlignment: Text.AlignHCenter
        }
    }

    width: 300
    height: 40
}
