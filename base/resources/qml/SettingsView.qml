import QtQuick 2.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

Item {
    anchors.fill: parent

    property double startTime: 30
    Button {
        id: serialHandlerOn
        x: 8
        y: 8
        width: 163
        height: 35
        text: qsTr("turn on SerialHandler")
        onClicked : {
            root.serialHandlerOn();
        }
    }

    Button {
        id: serialHandlerOff
        x: 8
        y: 41
        width: 163
        height: 35
        text: qsTr("turn off SerialHandler")
        onClicked : {
            root.serialHandlerOff();
        }
    }

    Button {
        id: updaterOn
        x: 8
        y: 74
        width: 163
        height: 35
        text: "turn on updater"
        onClicked : {
            root.updaterOn();
        }
    }

    Button {
        id: updaterOff
        x: 8
        y: 105
        width: 163
        height: 35
        text: "turn off updater"
        onClicked : {
            root.updaterOff();
        }
    }

    Button {
        id: controllerHandlerOn
        x: 9
        y: 140
        height: 35
        text: qsTr("turn on controller handler")
        onClicked : {
            root.controllerHandlerOn();
        }
    }

    Button {
        id: controllerHandlerOff
        x: 9
        y: 175
        height: 35
        text: qsTr("turn off controller handler")
        onClicked : {
            root.controllerHandlerOff();
        }
    }

    Rectangle {
        id: serialHandlerIndicator
        x: 177
        y: 8
        width: 52
        height: 35
        color: root.colorSerialHandler;
        BusyIndicator {
            id: indicator1
            width: 52
            height: 35
            //anchors.fill
            running: root.activeSerialHandler;
        }
    }

    Rectangle {
        id: controllerHandlerIndicator
        x: 177
        y: 140
        width: 52
        height: 35
        color: root.colorControllerHandler;
        BusyIndicator {
            id: indicator2
            x: 0
            y: 0
            width: 52
            height: 35
            //anchors.fill
            running: root.activeControllerHandler;
        }
    }

    Rectangle {
        id: updaterIndicator
        x: 178
        y: 74
        width: 51
        height: 35
        color: root.colorUpdater;
        BusyIndicator {
            id: indicator3
            width: 51
            height: 35
            running: root.activeUpdater;
        }
    }

    Button {
        id: pauseThreads
        x: 9
        y: 231
        width: 163
        height: 35
        text: qsTr("pause threads")
        onClicked : {
            root.pauseAllThreads();
        }
    }

    Button {
        id: resumeThreads
        x: 9
        y: 267
        width: 163
        height: 35
        text: qsTr("resume threads")
        onClicked : {
            root.resumeAllThreads();
        }
    }

    Button {
        id: connectDevice
        x: 400
        y: 54
        text: qsTr("connect device")
        onClicked: {
            root.connectDevice()
        }
    }

    Button {
        id: connectController
        x: 249
        y: 54
        text: qsTr("connect controllers")
    }

    Countdown {
        x: 309
        y: 123
    }

    RadioButton {
        id: radioButton
        x: 55
        y: 338
        text: qsTr("Radio Button")
    }
}
