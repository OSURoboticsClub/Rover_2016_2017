import QtQuick 2.0
import QtQuick.Controls 2.0
import QtPositioning 5.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import QtQuick.Templates 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls 1.0
import QtQuick.Controls 1.4


Item {
    property int t
    id: item1
    width: 200
    height: 600


    Gauge {
        id: leftMotor
        x: 0
        y: 36
        width: parent.width * 0.5
        height: parent.height * 0.3
        scale: 1.0
        antialiasing: true
        maximumValue: 13
        value: 5
        visible: true
    }

    Gauge {
        id: rightMotor
        x: 100
        y: 36
        width: parent.width * 0.5
        height: parent.height * 0.3
        anchors.leftMargin: 0
        transformOrigin: Item.Center
        antialiasing: true
        anchors.left: leftMotor.right;
        enabled: true
        visible: true
        value: 10
        rotation: 0
        maximumValue: 13
        clip: false
    }

    Item {
        id: voltageGuage
        property int min: 20
        property int max: 25
        property double value: testVoltProgressBar
        property color color: "green"

        width: parent.width; height: 23
        //        clip: true
        Rectangle {
            id: border
            anchors.fill: parent
            anchors.bottomMargin: 2
            anchors.rightMargin: 2
            color: "transparent"
            border.width: 2
            border.color: parent.color
        }
        Rectangle {
            id: progress
            anchors.top: border.top
            anchors.bottom: border.bottom
            anchors.left: border.left
            anchors.margins: 2

            width: (parent.width) * ((voltageGuage.value - voltageGuage.min)/(voltageGuage.max - voltageGuage.min));
            color: "red"

        }
    }

    ColumnLayout {
        id: columnLayout1
        anchors.fill: parent
    }

    Item {
        id: timerBackwards
        x: 0
        y: 300
        width: 200
        height: 200
        Loader {
            id: thisTimer
            anchors.fill: parent
            source: "countDownTimer.qml"
        }
    }

    Image {
        id: rectangle1
        x: 0
        y: 501
        width: parent.width
        height: 91
        source: "osurclogo.png"
    }
}

//    TextInput {
//        id: time
//        font.pixelSize: 30
//        text: "--"
//        x: 255
//        y: 267
//    }
//    TextInput {
//        id: mins
//        font.pixelSize: 30
//        text: "--"
//        x: 210
//        y: 267
//    }
//    TextInput {
//        id: hours
//        font.pixelSize: 30
//        text: "--"
//        x: 175
//        y: 267
//    }
//    Timer{
//        id: countdownTimer
//        interval: 1000
//        running: time.text > 0 || mins.text > 0 || hours.text > 0
//        repeat: true
//        onTriggered: {
//            if (time.text == 0 && (mins.text == 0 || mins.text == "--") && (hours.text == 0|| hours.text == "--")){
//                mins.text = "--"
//                time.text = "--"
//                hours.text = "--"
//            }
//            else if (time.text == 0 && mins.text == 0){
//                hours.text = --hours.text
//                time.text = "59"
//                mins.text = "59"
//            }
//            else if (time.text == 0){
//                mins.text = --mins.text
//                time.text = "59"
//            }
//            else{
//                time.text = --time.text
//            }
//        }

//    }

