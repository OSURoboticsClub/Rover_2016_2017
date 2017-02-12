import QtQuick 2.0
import QtQuick.Controls 2.0
import QtPositioning 5.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Templates 2.0


Item {
    property int t
    id: item
    width: 500; height: 100
    signal debugTest(string msg)
    signal closeMainWindow()
    MouseArea {
        anchors.rightMargin: -1
        anchors.bottomMargin: -285
        anchors.leftMargin: 1
        anchors.topMargin: 1
        anchors.fill: parent
        //onClicked: item.debugTest("hello")
        onClicked: item.closeMainWindow()
    }

    Rectangle{
        x: 362
        y: 1
        width: 100
        height: 100
        scale: 1
        enabled: true

    }

    Gauge {
        id: rightMotor
        x: 125
        y: 250
        width: 50
        height: 100
        enabled: true
        visible: true
        value: 10
        rotation: 0
        maximumValue: 13
        clip: false
    }

    Gauge {
        id: voltometer
        x: 50
        y: 75
        width: 100
        height: 100
        scale: 1
        enabled: false
        value: 75
        maximumValue: 100
        objectName: "voltometer"

    }

    StatusIndicator {
        id: roverMoving
        x: 179
        y: 1
        width: 20
        height: 20
        color: "#24f10a"
        active: true
    }

    StatusIndicator {
        id: unknownCommand
        x: 1
        y: 1
        width: 20
        height: 20
        color: "#fb0d0d"
        active: true
    }

    Gauge {
        id: leftMotor
        x: 25
        y: 250
        width: 50
        height: 100
        antialiasing: true
        maximumValue: 13
        value: 5
        visible: true
    }
    
    Text {
        id: cameraSelect
        x: 50
        y: 425
        width: 100
        height: 100
        text: qsTr("Camera 3")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
    }

    Gauge {
        id: accelerometer
        x: 50
        y: 575
        width: 100
        height: 100
        value: 63
        maximumValue: 100
    }

    Text {
        id: errorText
        x: 1
        y: 25
        width: 25
        height: 20
        text: qsTr("Error")
        font.pixelSize: 8
    }

    Text {
        id: moveText
        x: 179
        y: 25
        width: 20
        height: 20
        text: qsTr("Moving")
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 8
    }

    Text {
        id: voltText
        x: 65
        y: 181
        width: 70
        height: 15
        text: qsTr("Voltometer")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: leftText
        x: 21
        y: 356
        text: qsTr("Left Motor")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: rightText
        x: 117
        y: 356
        text: qsTr("Right Motor")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: accelText
        x: 60
        y: 681
        text: qsTr("Accelerometer")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }


}
