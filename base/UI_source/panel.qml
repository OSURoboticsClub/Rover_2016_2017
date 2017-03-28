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



    Item {
        id: voltageGuage
        property int min: 20
        property int max: 25
        property double value: testVoltProgressBar
        property color color: "black"

        width: parent.width; height: 37
        //        clip: true
        Rectangle {
            id: borderVoltage
            height: 51
            anchors.fill: parent
            anchors.bottomMargin: 2
            anchors.rightMargin: 2
            color: "transparent"
            border.width: 2
            border.color: parent.color
        }
        Rectangle {
            id: progressVoltage
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
        id: logo
        x: 0
        y: 501
        width: parent.width
        height: 91
        source: "Logo0.png"
        property int logoNumber: 0
        MouseArea {
            anchors.fill: parent
            onClicked: {
                logo.logoNumber++;
                logo.source = "Logo" + (logo.logoNumber % 3) + ".png";
            }
        }
    }
    CircularGauge {
        id: speedGuage
        x: 0
        y: 43
        width: 388
        height: 251
        minimumValue: 0
        maximumValue: 20
        stepSize: 1
        value: root.gps_speed
        Text {
            font.family: "Verdana"
            font.pointSize: 16
            anchors.verticalCenterOffset: -40
            anchors.horizontalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.gps_speed + " UPS"
        }

        style: CircularGaugeStyle {
            minorTickmarkCount: 5
            tickmarkStepSize: 5
            minimumValueAngle: -90
            maximumValueAngle: 90
            needle: Rectangle {
                implicitWidth: outerRadius * 0.03
                implicitHeight: outerRadius * 1.02
                antialiasing: true
                color: "black"
            }
            foreground: Item {
                Rectangle {
                    width: outerRadius * 0.2
                    height: width
                    radius: width / 2
                    color: "black"
                    anchors.centerIn: parent
                }
            }
            tickmarkLabel: Text {
                font.pixelSize: Math.max(6, outerRadius * 0.1)
                text: styleData.value
                color: styleData.value >= (speedGuage.maximumValue * .8) ? "red" : "green"
                antialiasing: true
            }
            tickmark: Rectangle {
                visible: styleData.value < speedGuage.maximumValue || styleData.value % 10 == 0
                implicitWidth: outerRadius * 0.02
                antialiasing: true
                implicitHeight: outerRadius * 0.11
                color: styleData.value >= (speedGuage.maximumValue * .8) ? "red" : "green"
            }
            minorTickmark: Rectangle {
                visible: styleData.value < speedGuage.maximumValue
                implicitWidth: outerRadius * .01
                antialiasing: true
                implicitHeight: outerRadius * .03
                color: styleData.value >= (speedGuage.maximumValue * .8) ? "red" : "green"
            }
        }
    }

}
