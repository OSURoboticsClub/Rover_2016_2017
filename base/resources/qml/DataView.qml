import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.3

Item {
    id: dataView
    height: parent.height
    width: parent.width

    property int largeFontSize: 15
    property int smallFontSize: 12    
    
    Column {
        id: dataCol1
        width: parent.width/2
        height: parent.height
        anchors.left: parent.left
        

        
        Text {
            id: batteryVoltage
            text: qsTr("Battery Voltage: " + root.battery_voltage)
            verticalAlignment: Text.AlignTop
            font.pixelSize: dataView.largeFontSize
        }

        Text {
            id: driveMotorPower
            text: qsTr("Drive Motor Power: ")
            font.pixelSize: largeFontSize
        }

        Text {
            id: lfDrive
            text: qsTr("   Left Front: " + root.l_f_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: lmDrive
            text: qsTr("   Left Middle: " + root.l_m_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: lbDrive
            text: qsTr("   Left Back: " + root.l_b_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: rfDrive
            text: qsTr("   Right Front: " + root.r_f_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: rmDrive
            text: qsTr("   Right Middle: " + root.r_m_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: rbDrive
            text: qsTr("   Right Back: " + root.r_b_drive)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: swerveDriveState
            text: qsTr("Swerve Drive State: " + root.swerve_state)
            wrapMode: Text.NoWrap
            font.pixelSize: largeFontSize
        }

        Text {
            id: armMotors
            text: qsTr("Arm Motors: ")
            font.pixelSize: largeFontSize
        }
        Text {
            id: m1
            text: qsTr("   Motor 1: " + root.arm_motor_1)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: m2
            text: qsTr("   Motor 2: " + root.arm_motor_2)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: m3
            text: qsTr("   Motor 3: " + root.arm_motor_3)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: m4
            text: qsTr("   Motor 4: " + root.arm_motor_4)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: m5
            text: qsTr("   Motor 5: " + root.arm_motor_5)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: potentiometers
            text: qsTr("Potentometers:")
            font.pixelSize: largeFontSize
        }
        Text {
            id: pot1
            text: qsTr("   Pot 1: " + root.pot_1)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: pot2
            text: qsTr("   Pot 2: " + root.pot_2)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: pot3
            text: qsTr("   Pot 3: " + root.pot_3)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: pot4
            text: qsTr("   Pot 4: " + root.pot_4)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: pot5
            text: qsTr("   Pot 5: " + root.pot_5)
            font.pixelSize: dataView.smallFontSize
        }

    }
    
    Column {
        id: dataCol2
        height: parent.height
        width: parent.width / 2
        anchors.left: dataCol1.right

        Text {
            id: selectedCamera
            text: qsTr("Selected Camera: " + root.selected_camera)
            font.pixelSize: largeFontSize
        }

        Text {
            id: callsign
            font.pixelSize: largeFontSize
        }

        Text {
            id: magnetomer
            text: qsTr("Magnetomer: ")
            font.pixelSize: largeFontSize
        }
        Text {
            id: magX
            text: qsTr("   X: " + root.mag_x)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: magY
            text: qsTr("   Y: " + root.mag_y)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: magZ
            text: qsTr("   Z: " + root.mag_z)
            font.pixelSize: dataView.smallFontSize
        }


        Text {
            id: gyroscope
            text: qsTr("Gyroscope: ")
            font.pixelSize: dataView.largeFontSize
        }
        Text {
            id: gyroX
            text: qsTr("   X: " + root.gyro_x)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: gyroY
            text: qsTr("   Y: " + root.gyro_y)
            font.pixelSize: dataView.smallFontSize
        }
        Text {
            id: gyroZ
            text: qsTr("   Z: " + root.gyro_z)
            font.pixelSize: dataView.smallFontSize
        }

        Text {
            id: gpioDir
            text: qsTr("GPIO direction: " + root.gpio_dir)
            font.pixelSize: dataView.largeFontSize
        }
        Text {
            id: gpioOutValue
            text: qsTr("GPIO out value: " + root.gpio_out)
            font.pixelSize: dataView.largeFontSize
        }
        Text {
            id: gpioReadState
            text: qsTr("GPIO read state: " + root.gpio_state)
            font.pixelSize: dataView.largeFontSize
        }

        Text {
            id: debugInfo
            text: qsTr("Debugging info: " + root.debug_str_data)
            font.pixelSize: dataView.largeFontSize
        }

        Text {
            id:buildInfo
            text: qsTr("Build info: " + root.build_info_data)
            font.pixelSize: dataView.largeFontSize
        }
        Text {
            id: latitude
            text: qsTr("Latitude: " + root.latitude)
            font.pixelSize: dataView.largeFontSize
        }
        Text {
            id:longitude
            text: qsTr("Longitude: " + root.longitude)
            font.pixelSize: dataView.largeFontSize
        }
        Text {
            id:gps_heading
            text: qsTr("Heading: " + root.gps_heading)
            font.pixelSize: dataView.largeFontSize
        }
    }


}
