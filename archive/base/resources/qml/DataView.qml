import QtQuick 2.1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Row {
    id: dataView

    property int largeFontSize: 20
    property int smallFontSize: 15
    spacing: 50
    anchors.margins: 10

    Column {
        id: dataCol1
        spacing: 10

        Label {
            id: batteryVoltage
            text: qsTr("Battery Voltage: " + root.battery_voltage)
            font.pixelSize: dataView.largeFontSize
        }

        Label {
            id: driveMotorPower
            text: qsTr("Drive Motor Power: ")
            font.pixelSize: dataView.largeFontSize
        }

        Label {
            id: lfDrive
            text: qsTr("   Left Front: " + root.l_f_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Label {
            id: lmDrive
            text: qsTr("   Left Middle: " + root.l_m_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Label {
            id: lbDrive
            text: qsTr("   Left Back: " + root.l_b_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Label {
            id: rfDrive
            text: qsTr("   Right Front: " + root.r_f_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Label {
            id: rmDrive
            text: qsTr("   Right Middle: " + root.r_m_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Label {
            id: rbDrive
            text: qsTr("   Right Back: " + root.r_b_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Label {
            id: swerveDriveState
            text: qsTr("Swerve Drive State: " + root.swerve_state)
            font.pixelSize: dataView.largeFontSize
        }

        Label {
            id: armMotors
            text: qsTr("Arm Motors: ")
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id: m1
            text: qsTr("   Motor 1: " + root.arm_motor_1)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: m2
            text: qsTr("   Motor 2: " + root.arm_motor_2)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: m3
            text: qsTr("   Motor 3: " + root.arm_motor_3)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: m4
            text: qsTr("   Motor 4: " + root.arm_motor_4)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: m5
            text: qsTr("   Motor 5: " + root.arm_motor_5)
            font.pixelSize: dataView.smallFontSize
        }


    }
    
    Column {
        id: dataCol2
        spacing: 10

        Label {
            id: selectedCamera
            text: qsTr("Selected Camera: " + root.selected_camera)
            font.pixelSize: dataView.largeFontSize
        }
        /*
        Label {
            id: callsign
            font.pixelSize: dataView.largeFontSize
        }
        */
        Label {
            id: potentiometers
            text: qsTr("Potentometers:")
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id: pot1
            text: qsTr("   Pot 1: " + root.pot_1)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: pot2
            text: qsTr("   Pot 2: " + root.pot_2)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: pot3
            text: qsTr("   Pot 3: " + root.pot_3)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: pot4
            text: qsTr("   Pot 4: " + root.pot_4)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: pot5
            text: qsTr("   Pot 5: " + root.pot_5)
            font.pixelSize: dataView.smallFontSize
        }


        Label {
            id: magnetomer
            text: qsTr("Magnetometer: ")
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id: magX
            text: qsTr("   X: " + root.mag_x)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: magY
            text: qsTr("   Y: " + root.mag_y)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: magZ
            text: qsTr("   Z: " + root.mag_z)
            font.pixelSize: dataView.smallFontSize
        }


        Label {
            id: gyroscope
            text: qsTr("Gyroscope: ")
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id: gyroX
            text: qsTr("   X: " + root.gyro_x)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: gyroY
            text: qsTr("   Y: " + root.gyro_y)
            font.pixelSize: dataView.smallFontSize
        }
        Label {
            id: gyroZ
            text: qsTr("   Z: " + root.gyro_z)
            font.pixelSize: dataView.smallFontSize
        }
        /*
        Label {
            id: gpioDir
            text: qsTr("GPIO direction: " + root.gpio_dir)
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id: gpioOutValue
            text: qsTr("GPIO out value: " + root.gpio_out)
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id: gpioReadState
            text: qsTr("GPIO read state: " + root.gpio_state)
            font.pixelSize: dataView.largeFontSize
        }

        Label {
            id: debugInfo
            text: qsTr("Debugging info: " + root.debug_str_data)
            font.pixelSize: dataView.largeFontSize
        }

        Label {
            id:buildInfo
            text: qsTr("Build info: " + root.build_info_data)
            font.pixelSize: dataView.largeFontSize
        }
        */
    }
    Column {
        id: dataCol3
        spacing: 10
        Label {
            id: latitude
            text: qsTr("Latitude: " + root.latitude)
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id:longitude
            text: qsTr("Longitude: " + root.longitude)
            font.pixelSize: dataView.largeFontSize
        }
        Label {
            id:gps_heading
            text: qsTr("Heading: " + root.gps_heading)
            font.pixelSize: dataView.largeFontSize
        }

        Countdown {
        }
    }

}
