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
            root._serialHandlerOn();
        }
    }

    Button {
        id: serialHandlerOff
        x: 8
        y: 41
        height: 35
        text: qsTr("turn off SerialHandler")
        onClicked : {
            root._serialHandlerOff();
        }
    }

    Button {
        id: updaterOn
        x: 8
        y: 74
        height: 35
        text: "turn on updater"
        onClicked : {
            root._updaterOn();
        }
    }

    Button {
        id: updaterOff
        x: 8
        y: 105
        height: 35
        text: "turn off updater"
        onClicked : {
            root._updaterOff();
        }
    }

    Button {
        id: controllerHandlerOn
        x: 9
        y: 140
        height: 35
        text: qsTr("turn on controller handler")
        onClicked : {
            root._controllerHandlerOn();
        }
    }

    Button {
        id: controllerHandlerOff
        x: 9
        y: 175
        height: 35
        text: qsTr("turn off controller handler")
        onClicked : {
            root._controllerHandlerOff();
        }
    }

    Button {
        id: allThreadsClose
        x: 236
        y: 93
        height: 35
        text: qsTr("turn off all threads")
        onClicked : {
            root._allThreadsClose();
        }
    }

    Rectangle {
        id: serialHandlerIndicator
        x: 156
        y: 8
        width: 52
        height: 35
        color: root.colorSerialHandler;
    }
}
