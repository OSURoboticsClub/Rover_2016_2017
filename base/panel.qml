import QtQuick 2.0
import QtQuick.Controls 2.0
import QtPositioning 5.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Templates 2.0

Item {
    property int t
    id: item1
    width: 200
    height: 600
    /*
    Gauge {
        id: voltometer
        y: 486
        anchors{
            bottom: parent.bottom
            left: parent.left
        }

        width: parent.width * .5
        height: parent.height * .2
        anchors.bottomMargin: -6
        anchors.leftMargin: 445
        scale: 1
        enabled: false
        value: 75
        maximumValue: 100
    }

    StatusIndicator {
        id: roverMoving
        x: 399
        anchors{
            right: parent.right
            top: parent.top
        }

        width: 20
        height: 20
        color: "#24f10a"
        anchors.rightMargin: -219
        anchors.topMargin: 7
        active: true
    }

    StatusIndicator {
        id: unknownCommand
        anchors{
            left: parent.left
            top: parent.top
        }
        width: 20
        height: 20
        color: "#fb0d0d"
        anchors.leftMargin: 219
        anchors.topMargin: 7
        active: true
    }
    
    Text {
        id: cameraSelect
        y: 280
        anchors{
            bottom: gyro.top
            left: parent.left
        }
        width: parent.width * .5
        height: parent.height * .2
        text: qsTr("Camera 3")
        anchors.bottomMargin: -223
        anchors.leftMargin: 236
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
    }

    Gauge {
        id: accelerometer
        y: 322
        anchors{
            bottom: voltometer.top
            left: parent.horizontalCenter
        }
        width: parent.width * .5
        height: parent.height * .2
        anchors.bottomMargin: 23
        anchors.leftMargin: 219
        value: 63
        maximumValue: 100
    }

    Gauge {
        id: gyro
        y: 184
        anchors{
            bottom: accelerometer.top
            left: parent.left
        }
        width: parent.width * .5
        height: parent.height * .2
        anchors.bottomMargin: 11
        anchors.leftMargin: 520
        value: 63
        maximumValue: 100
    }

    Text {
        id: errorText
        anchors{
            top: unknownCommand.bottom
            left: parent.left
        }
        width: 25
        height: 20
        text: qsTr("Error")
        anchors.leftMargin: 219
        anchors.topMargin: 7
        font.pixelSize: 8
    }

    Text {
        id: moveText
        x: 394
        anchors{
            top: roverMoving.bottom
            right: parent.right
        }
        width: 25
        height: 20
        text: qsTr("Moving")
        anchors.rightMargin: -219
        anchors.topMargin: 7
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 8
    }

    Text {
        id: voltText
        anchors{
            top: voltometer.bottom
            left: parent.left
        }

        width: 70
        height: 15
        text: qsTr("Voltometer")
        anchors.leftMargin: 319
        anchors.topMargin: 7
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

    Text {
        id: leftText
        anchors{
            top: leftMotor.bottom
            left: parent.left
        }

        text: qsTr("Left Motor")
        anchors.leftMargin: 219
        anchors.topMargin: 7
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: rightText
        x: 353
        anchors{
            top: rightMotor.bottom
            right: parent.right
        }
        text: qsTr("Right Motor")
        anchors.rightMargin: -219
        anchors.topMargin: 7
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: accelText
        anchors{
            top: accelerometer.bottom
            left: parent.left
        }
        text: qsTr("Accelerometer")
        anchors.leftMargin: 319
        anchors.topMargin: 7
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: gyroText
        anchors{
            top: gyro.bottom
            left: parent.left
        }
        text: qsTr("Gyro")
        anchors.leftMargin: 319
        anchors.topMargin: 7
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    RowLayout {
        id: rowLayout1
        x: 236
        y: 74
        width: 100
        height: 100
        anchors.right: parent.right
        anchors.rightMargin: -136
    }
    */
    ColumnLayout {
        id: columnLayout1
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        RowLayout {
            id: rowLayout2
            Layout.fillWidth: true


            Gauge {
                id: leftMotor
                antialiasing: true
                maximumValue: 13
                value: 5
                visible: true
            }
            Gauge {
                id: rightMotor
                antialiasing: true
                enabled: true
                visible: true
                value: 10
                rotation: 0
                maximumValue: 13
                clip: false
            }
        }
        Gauge {
            id: accelerometer
            width: parent.width * .5
            height: parent.height * .2
            Layout.fillWidth: true
            value: 63
            maximumValue: 100
        }

       Gauge {
        id: voltometer
        width: parent.width * .5
        height: parent.height * .2
        Layout.fillWidth: true
        scale: 1
        value: 75
        maximumValue: 100
        }
       Gauge {
           id: gyro

           width: parent.width * .5
           height: parent.height * .2
           Layout.fillWidth: true
           value: 63
           maximumValue: 100
       }
    Text {
        id: cameraSelect

        width: parent.width * .5
        height: parent.height * .2
        text: qsTr("Camera 3")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
    }
   }
}
