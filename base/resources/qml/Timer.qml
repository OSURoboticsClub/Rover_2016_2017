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

Rectangle {
    id: timer
    width: 200
    height: 200
    border.width: 5
    border.color: "black"

    property int timeElapsed: 0
    property variant timeText: [0,0,0]
    property bool timerOn: false
    property int startTime: 0

    function convertTime(time) {
        var hours = 0;
        var minutes = 0;
        var seconds = 0;
        while (time > 3600000) {
            hours++;
            time -= 3600000;
        }
        while (time > 60000){
            minutes++;
            time -= 60000;
        }
        while (time > 1000){
            seconds++;
            time -= 1000;
        }
        timer.timeText[0] = hours;
        timer.timeText[1] = minutes;
        timer.timeText[2] = seconds;
    }

    function grabTime() {
        var hours = Number(hours2.text);
        var minutes = Number(minutes2.text);
        var seconds = Number(seconds2.text);

        var time = 0;
        time += (3600000 * hours);
        time += (60000 * minutes);
        time += (1000 * seconds);

        timer.startTime = time;
        convertTime(time);
    }

    function setTime() {
        hours2.text = timer.timeText[0];
        minutes2.text = timer.timeText[1];
        seconds2.text = timer.timeText[2];
    }



    Timer {
        id: localTimer
        interval: 200
        running: false
        repeat: true
        onTriggered: {
            timer.timeElapsed += 200;
            if ((timer.startTime - timer.timeElapsed) > 0) {
                timer.convertTime((timer.startTime - timer.timeElapsed));
                timer.setTime();
            }
            else {
                hours2.text = "---";
                minutes2.text = "---";
                seconds2.text = "---";
            }

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

    }

    Button {
        id: pauseTime
        x: 8
        y: 90
        width: 184
        height: 28
        text: qsTr("pause")
        onClicked : {
            localTimer.running = false;
        }
    }

    Button {
        id: setTime
        x: 8
        y: 56
        width: 184
        height: 28
        text: qsTr("set time")
        onClicked : {
            timer.grabTime();
            timer.setTime();
        }
    }

    Button {
        id: startTime
        x: 8
        y: 124
        width: 184
        height: 28
        text: qsTr("start")
        onClicked : {
            localTimer.running = true;
        }
    }

    Button {
        id: reset
        x: 8
        y: 158
        width: 184
        height: 28
        text: qsTr("reset")
        onClicked: {
            timer.timeElapsed = 0;
            timer.startTime = 0;
            hours2.text = "";
            minutes2.text = "";
            seconds2.text = "";
            localTimer.running = false;

        }

    }
}
