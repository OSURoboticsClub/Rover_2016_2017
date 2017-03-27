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

import "simpleGuage" as Guage


Item {
    property int t
    id: item1
    width: 200
    height: 600


    Item {
        id: voltageGuage
        property int min: 20
        property int max: 25
        property double value: testVoltProgressBar
        property color color: "black"

        width: parent.width; height: 37
        //        clip: true
        Rectangle {
            id: border
            height: 51
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
            color: "blue"

        }

        Text {
            id: voltageGuageLabel
            x: 71
            y: 10
            anchors.leftMargin: 100
            text: qsTr("Voltage")
            anchors.horizontalCenter: border.horizontalCenter
            font.bold: true
            font.family: "Tahoma"
            font.pixelSize: 13
        }
    }

    ColumnLayout {
        id: columnLayout1
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
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

