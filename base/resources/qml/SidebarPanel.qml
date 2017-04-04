import QtQuick 2.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtPositioning 5.7

ColumnLayout {
    id: sidebarPanel
    anchors.fill: parent

    AttitudeIndicator{
        id: indicator
        width: 100
        height: 100
        Layout.minimumHeight: 100
        Layout.fillWidth: true
    }

    CircularGauge {
        id: circularGauge
        Layout.minimumWidth: 0
        Layout.preferredHeight: 200
        Layout.minimumHeight: 150
        Layout.fillWidth: true
    }


    MultiGauge {
        id: multiGauge
        Layout.minimumHeight: 100
        Layout.preferredHeight: 150
        Layout.fillWidth: true
        model: [root.arm_motor_1, root.arm_motor_2, root.arm_motor_3,
            root.arm_motor_4, root.arm_motor_5]
    }

    MultiGauge {
        id: multiGauge1
        Layout.minimumHeight: 100
        Layout.preferredHeight: 150
        Layout.fillWidth: true
        model: [root.l_f_drive, root.l_m_drive, root.l_b_drive,
            root.r_f_drive, root.r_m_drive, root.r_b_drive]
    }





}

