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
        id: indicator
        Layout.minimumHeight: 200
        Layout.fillWidth: true
    }

    CircularGauge {
        maximumValue: 20
        Layout.preferredHeight: 200
        Layout.fillWidth: true

        style: CircularGaugeStyle {
            minorTickmarkCount: 5
            tickmarkStepSize: 5
        }
    }


    Label {
        id: text1
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
        id: text2
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
        id: label
        text: "Battery Voltage"
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
    }

    Rectangle {
        id: voltageGauge
        property int limit: 25
        property double value: 20000 / 1000
        Layout.fillWidth: true
        Layout.minimumHeight: 40

        color: Material.foreground

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.value * parent.width / parent.limit
            color: parent.width/2 > width ?
                     Material.color(Material.Red):
                        Material.accent
        }

        Label {
            text: parent.value + " V"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

    }

    Item {
        id: row
        Layout.minimumHeight: 50
        Layout.fillWidth: true

        StatusIndicator {
            id: statusIndicator
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
        }

        Label {
            id: text3
            text: "Status"
            anchors.verticalCenter: statusIndicator.verticalCenter
            anchors.left: statusIndicator.right
            anchors.leftMargin: 10
        }

        Rectangle {
            id: rectangle
            width: 40
            height: 40
            color: "#e32929"
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }

        Label {
            id: text4
            text: qsTr("Camera")
            horizontalAlignment: Text.AlignRight
            anchors.verticalCenter: text3.verticalCenter
            anchors.right: rectangle.left
            anchors.rightMargin: 10
        }
    }

}

