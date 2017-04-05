import QtQuick 2.1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Column {
    anchors.fill: parent

    property double startTime: 30

    Row {
        Label {
            id: label1
            text: "Serial"
            font.pixelSize: 12
        }
    }

    Row {
        id: row

        spacing: 10

        Switch {
            id: switch1
            text: qsTr("Serial")
        }

        ComboBox {
            id: comboBox
        }
    }

    Row {
        id: row1

        spacing: 10

        Switch {
            id: switch2
            text: "Updater"
        }

        Slider {
            id: slider
            value: 0.5
        }
    }
    Row {
        id: row2
        Switch {
            id: switch3

            text: qsTr("Switch")
        }
    }

}
