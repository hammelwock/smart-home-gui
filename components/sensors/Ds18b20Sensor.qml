import QtQuick 2.15

MySensor {
    value: "Температура: " + modelData.value + " °C"

    pin.infoText: "Индекс:"

    measuredQuantity.height: 0
    measuredQuantity.visible: false
}
