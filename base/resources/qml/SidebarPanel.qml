import QtQuick 2.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Controls 2.0
import QtPositioning 5.7

ColumnLayout {
    id: sidebarPanel
    //anchors.fill: parent

    AttitudeIndicator{
        id: attitudeIndicator
        Layout.minimumHeight: 200
        Layout.fillWidth: true
    }


    Label {
        id: armMotorLabel
        text: "Arm Motors"
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
    }

    MultiGauge {
        id: armMotorGauge
        Layout.minimumHeight: 70
        Layout.fillWidth: true
        limits: [-127, 127]
        model: [-50, 100, root.arm_motor_3,
            root.arm_motor_4, root.arm_motor_5]
    }

    Label {
        id: motorPowerLabel
        text: "Motor Power"
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
    }

    MultiGauge {
        id: motorPowerGauge
        Layout.minimumHeight: 70
        Layout.fillWidth: true
        limits: [0, 255]
        model: [10, 20, 150,
            root.r_f_drive, root.r_m_drive, root.r_b_drive]
    }

    Label {
        id: batteryVoltageLabel
        text: "Battery Voltage"
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
    }

    VoltageGuage{
        id: voltageGuage
        Layout.fillWidth: true
    }

    Item {
        id: statusItem
        Layout.minimumHeight: 50
        Layout.fillWidth: true

        StatusIndicator {
            id: statusIndicator
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: statusText
            text: "Status"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: statusIndicator.right
            anchors.leftMargin: 5
        }

        Image {
            id: cameraIcon
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.right: parent.right
            fillMode: Image.PreserveAspectFit
            source: "qrc:/img/placeholder-icon.svg"
        }

        Label {
            id: cameraText
            text: "Camera"
            horizontalAlignment: Text.AlignRight
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: cameraIcon.left
            anchors.rightMargin: 5
        }
    }
    CircularGauge {
            id: speedGuage
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.preferredHeight: 200
            Layout.fillWidth: true
            minimumValue: 0
            maximumValue: 20
            stepSize: 1
            value: root.gps_speed/1000
            Text {
                font.family: "Verdana"
                font.pointSize: 16
                anchors.verticalCenterOffset: -40
                anchors.horizontalCenterOffset: 0
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: root.gps_speed/1000 + " KPH"
                color: "white"
            }

            style: CircularGaugeStyle {
                minorTickmarkCount: 5
                tickmarkStepSize: 5
                minimumValueAngle: -90
                maximumValueAngle: 90
                needle: Rectangle {
                    implicitWidth: outerRadius * 0.03
                    implicitHeight: outerRadius * .85
                    antialiasing: true
                }
                foreground: Item {
                    Rectangle {
                        width: outerRadius * 0.2
                        height: width
                        radius: width / 2
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


