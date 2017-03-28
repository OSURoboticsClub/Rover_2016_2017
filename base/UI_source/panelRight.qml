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
    id: panelRight
    anchors.fill: parent
    Item {
        id: timerBackwards
        x: 0
        width: 200
        height: 200
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Loader {
            id: thisTimer
            anchors.fill: parent
            source: "countDownTimer.qml"
        }
    }

    Text {
        id: lat
        text: "Lat: [" + root.latitude + "]";
        anchors.left: parent.left
        anchors.leftMargin: 0
        font.pointSize: 16
        style: Text.Normal
        anchors.top: timerBackwards.bottom
        anchors.topMargin: 25

    }
    Text {
        id: lng
        text: "Lng: [" + root.longitude + "]";
        style: Text.Normal
        font.pointSize: 16
        font.family: "Tahoma"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: lat.bottom
        anchors.topMargin: 10

    }

}
