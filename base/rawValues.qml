import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.3

ColumnLayout {
    id: columnLayout
    anchors.fill: parent

    Text {
        id: batteryVoltage
        text: qsTr("Battery Voltage: " + root.battery_voltage)
        z: 1
        verticalAlignment: Text.AlignTop
        font.pixelSize: 15
    }

    Text {
        id: driveMotorPower
        text: qsTr("Drive Motor Power: ")
        font.pixelSize: 15
    }

    Text {
        id: lfDrive
        text: qsTr("   Left Front: " + root.l_f_drive)
        font.pixelSize: 12
    }

    Text {
        id: lmDrive
        text: qsTr("   Left Middle: " + root.l_m_drive)
        font.pixelSize: 12
    }

    Text {
        id: lbDrive
        text: qsTr("   Left Back: " + root.l_b_drive)
        font.pixelSize: 12
    }

    Text {
        id: rfDrive
        text: qsTr("   Right Front: " + root.r_f_drive)
        font.pixelSize: 12
    }

    Text {
        id: rmDrive
        text: qsTr("   Right Middle: " + root.r_m_drive)
        font.pixelSize: 12
    }

    Text {
        id: rbDrive
        text: qsTr("   Right Back: " + root.r_b_drive)
        font.pixelSize: 12
    }

    Text {
        id: swerveDriveState
        text: qsTr("Swerve Drive State: " + root.swerve_state)
        font.pixelSize: 15
    }

    Text {
        id: armMotors
        text: qsTr("Arm Motors: ")
        font.pixelSize: 15
    }
    Text {
        id: m1
        text: qsTr("   Motor 1: " + root.arm_motor_1)
        font.pixelSize: 12
    }
    Text {
        id: m2
        text: qsTr("   Motor 2: " + root.arm_motor_2)
        font.pixelSize: 12
    }
    Text {
        id: m3
        text: qsTr("   Motor 3: " + root.arm_motor_3)
        font.pixelSize: 12
    }
    Text {
        id: m4
        text: qsTr("   Motor 4: " + root.arm_motor_4)
        font.pixelSize: 12
    }
    Text {
        id: m5
        text: qsTr("   Motor 5: " + root.arm_motor_5)
        font.pixelSize: 12
    }

    Text {
        id: potentiometers
        text: qsTr("Potentometers:")
        font.pixelSize: 15
    }
    Text {
        id: pot1
        text: qsTr("   Pot 1: " + root.pot_1)
        font.pixelSize: 12
    }
    Text {
        id: pot2
        text: qsTr("   Pot 2: " + root.pot_2)
        font.pixelSize: 12
    }
    Text {
        id: pot3
        text: qsTr("   Pot 3: " + root.pot_3)
        font.pixelSize: 12
    }
    Text {
        id: pot4
        text: qsTr("   Pot 4: " + root.pot_4)
        font.pixelSize: 12
    }
    Text {
        id: pot5
        text: qsTr("   Pot 5: " + root.pot_5)
        font.pixelSize: 12
    }

    Text {
        id: selectedCamera
        text: qsTr("Selected Camera: " + root.selected_camera)
        font.pixelSize: 15
    }

    Text {
        id: callsign
        text: qsTr("Callsign: " + root.callsign_data)
        font.pixelSize: 15
    }

    Text {
        id: magnetomer
        text: qsTr("Magnetomer: ")
        font.pixelSize: 15
    }
    Text {
        id: magX
        text: qsTr("   X: " + root.mag_x)
        font.pixelSize: 12
    }
    Text {
        id: magY
        text: qsTr("   Y: " + root.mag_y)
        font.pixelSize: 12
    }
    Text {
        id: magZ
        text: qsTr("   Z: " + root.mag_z)
        font.pixelSize: 12
    }


    Text {
        id: gyroscope
        text: qsTr("Gyroscope: ")
        font.pixelSize: 15
    }
    Text {
        id: gyroX
        text: qsTr("   X: " + root.gyro_x)
        font.pixelSize: 12
    }
    Text {
        id: gyroY
        text: qsTr("   Y: " + root.gyro_y)
        font.pixelSize: 12
    }
    Text {
        id: gyroZ
        text: qsTr("   Z: " + root.gyro_z)
        font.pixelSize: 12
    }

    Text {
        id: gpioDir
        text: qsTr("GPIO direction: " + root.gpio_dir)
        font.pixelSize: 15
    }
    Text {
        id: gpioOutValue
        text: qsTr("GPIO out value: " + root.gpio_out)
        font.pixelSize: 15
    }
    Text {
        id: gpioReadState
        text: qsTr("GPIO read state: " + root.gpio_state)
        font.pixelSize: 15
    }

    Text {
        id: debugInfo
        text: qsTr("Debugging info: " + root.debug_str_data)
        font.pixelSize: 15
    }

    Text {
        id:buildInfo
        text: qsTr("Build info: " + root.build_info_data)
        font.pixelSize: 15
    }

    Column {
        id: column1
        x: 438
        y: 0
        width: 200
        height: 400
    }

}
