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
        model: [root.arm_motor_1, root.arm_motor_2, root.arm_motor_3,
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
        model: [root.l_f_drive, root.l_m_drive, root.l_b_drive,
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
    SpeedGuage {
        id: speedGuage
    }

}


