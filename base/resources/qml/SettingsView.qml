import QtQuick 2.1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

// TODO: id everything accurately

Column {

    Label {
        id: label1
        text: "Serial"
    }

    Row {
        id: row

        spacing: 10

        Switch {
            id: serialSwitch
            text: "Serial"
            onClicked: {
                checked ? serialHandlerOn() : serialHandlerOff()
            }
        }

        Label {
            text: "Serial port"
            anchors.verticalCenter: parent.verticalCenter
        }

        ComboBox {
            id: comboBox
        }
    }

    Label {
        text: "Packet Updates"
    }

    Row {
        id: row1

        spacing: 10

        Switch {
            id: updaterSwitch
            text: "Updater"
            onClicked: {
                checked ? updaterOn() : updaterOff()
            }
        }

        Label {
            text: "Refresh rate"
            anchors.verticalCenter: parent.verticalCenter
        }

        Slider {
            id: slider
            value: 0.5
        }
    }

    Label {
        text: "User Inputs"
    }

    Row {
        id: row2
        Switch {
            id: controllerSwitch
            text: "Controllers"
            onClicked: {
                checked ? controllerHandlerOn() : controllerHandlerOff()
            }
        }
    }

}
