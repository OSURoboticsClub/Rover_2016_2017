import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    anchors.fill: parent
    Button {
        id: serialHandlerOn
        x: 8
        y: 8
        height: 35
        text: qsTr("turn on SerialHandler")
        objectName: "serialHandlerOn"
        onClicked : {
            window._serialHandlerOn();
        }
    }

    Button {
        id: serialHandlerOff
        x: 8
        y: 41
        height: 35
        text: qsTr("turn of SerialHandler")
    }

    Button {
        id: updaterOn
        x: 8
        y: 74
        height: 35
        text: "turn on updater"
    }

    Button {
        id: updaterOff
        x: 8
        y: 105
        height: 35
        text: "turn off updater"
    }

    Button {
        id: controllerHandlerOn
        x: 9
        y: 140
        height: 35
        text: qsTr("turn on controller handler")
    }

    Button {
        id: controllerHandlerOff
        x: 9
        y: 175
        height: 35
        text: qsTr("turn off controller handler")
    }
}
