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
    id: sidebarPanel
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
            id: borderRect
            anchors.fill: parent
            anchors.bottomMargin: 2
            anchors.rightMargin: 2
            color: "transparent"
            border.width: 2
            border.color: parent.color
        }
        Rectangle {
            id: progress
            anchors.top: borderRect.top
            anchors.bottom: borderRect.bottom
            anchors.left: borderRect.left
            anchors.margins: 2

            width: (parent.width) * ((voltageGuage.value - voltageGuage.min)/(voltageGuage.max - voltageGuage.min));
            color: "red"

        }
    }

    ColumnLayout {
        id: columnLayout1
        anchors.fill: parent
    }
    
    Timer{id: timer}

    Image {
        id: rectangle1
        x: 0
        y: 501
        width: parent.width
        height: 91
        source: "qrc:/img/osurc-logo.png"
    }
}

