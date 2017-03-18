import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls 1.4

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
        objectName: "serialHandlerOn"
        onClicked : {
            root._serialHandlerOn();
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
            root._serialHandlerOff();
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
            root._updaterOn();
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
            running: root.activeSeriaHandler;
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
            root._pauseAllThreads();
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
            root._resumeAllThreads();
        }
    }

    Button {
        id: setRoverPos
        text: qsTr("change rover coords")
        x: 249
        y: 15
        onClicked: {
            root.latitude = 38.4063
            root.longitude = -110.792286
            root.gps_heading = 60
            console.log(gps.coords)
        }

    }

    Button {
        id: upVolt
        x: 26
        y: 385
        text: qsTr("upVolt")
        onClicked: {
            root.testVoltProgressBar += .3;
        }
    }

    Button {
        id: downVolt
        x: 149
        y: 385
        text: qsTr("downVolt")
        onClicked: {
            root.testVoltProgressBar -= .3;
        }
    }

    TextInput {
                id: time
                font.pixelSize: 30
                text: "--"
                x: 255
                y: 267
            }
    TextInput {
                id: mins
                font.pixelSize: 30
                text: "--"
                x: 210
                y: 267
            }
    TextInput {
                id: hours
                font.pixelSize: 30
                text: "--"
                x: 175
                y: 267
            }
    Timer{
        id: countdownTimer
        interval: 1000
        running: time.text > 0 || mins.text > 0 || hours.text > 0
        repeat: true
        onTriggered: {
            if (time.text == 0 && (mins.text == 0 || mins.text == "--") && (hours.text == 0|| hours.text == "--")){
                mins.text = "--"
                time.text = "--"
                hours.text = "--"
            }
            else if (time.text == 0 && mins.text == 0){
                hours.text = --hours.text
                time.text = "59"
                mins.text = "59"
            }
            else if (time.text == 0){
                mins.text = --mins.text
                time.text = "59"
            }
            else{
                time.text = --time.text
            }
        }

    }
}
