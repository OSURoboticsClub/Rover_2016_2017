import QtQuick 2.0
import QtQuick.Controls 2.0
import QtPositioning 5.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import QtQuick.Templates 2.0


Item {
    property int t
    id: item1
    width: 200
    height: 600
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

    Rectangle {
        id: countDownTimer
        x: 0
        y: 355
        width: 200
        height: 200
        border.width: 5
        border.color: "black"

        property double timeElapsed: 30000
        property variant timeText: [0,0,0]

        function convertTime(time) {
            var hours = 0;
            var minutes = 0;
            var seconds = 0;
            while (time > 3600000) {
                hours++;
                time -= 3600000;
            }
            while (minutes > 60000){
                minutes++;
                time -= 60000;
            }
            while (time > 1000){
                seconds++;
                time -= 1000;
            }
            countDownTimer.timeText[0] = hours;
            countDownTimer.timeText[1] = minutes;
            countDownTimer.timeText[2] = seconds;
        }

        function grabTime() {
            var hours = parseInt(hours2.text);
            var minutes = parseInt(minutes2.text);
            var seconds = parseInt(seconds2.text);

            var time = 0;
            time += (3600000 * hours);
            time += (60000 * minutes);
            time += (1000 * seconds);

            convertTime(time);
        }

        function setTime() {
            hours2.text = countDownTimer.timeText[0];
            minutes2.text = countDownTimer.timeText[1];
            seconds2.text = countDownTimer.timeText[0];
        }



        Timer {
            interval: 500
            running: true
            repeat: true
            onTriggered: {
                countDownTimer.timeElapsed += 500;
                countDownTimer.setTime();
            }
        }


        Rectangle {
            x: 0
            y: 0
            width: 200
            height: 50
            border.width: 2
            border.color: "black"
            TextInput {
                id: hours2
                x: 0
                y: 0
                font.pixelSize: 30
                text: ""
                width: parent.width / 3.4
                height: 50

            }

            TextInput  {
                id: minutes2
                x: 71
                y: 0
                text: ""
                font.pixelSize: 30
                width: parent.width / 3.4
                height: 50

            }

            TextInput {
                id: seconds2
                x: 141
                y: 0

                font.pixelSize: 30
                text: ""
                width: parent.width / 3.4
                height: 50

            }

            Text {
                id: colonLeft
                x: 60
                y: 0
                text: qsTr(":")
                font.pixelSize: 36
            }
            Text {
                id: colonRight
                x: 129
                y: 0
                text: qsTr(":")
                font.pixelSize: 36
            }
        }



    }
}
