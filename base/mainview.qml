import QtQuick 2.0
import QtQuick.Controls 1.4
//import QtWebEngine 1.4
import QtWebEngine 1.3
import QtQuick.Layouts 1.3

Item {
    id: mainView
    //width: parent.width
    //height: parent.height
    property int j: 1
    TabView {
        id: mainTab
        width: parent.width
        height: parent.height
        //        anchors.fill: parent
        Tab {
            title: "map"

            WebEngineView {
                anchors.fill: parent
                url: "map.html"
            }
            /*           Item {
                id: map

            }
*/
        }

        Tab {
            id: settings
            title: "Settings"
            source: "Settings.qml"
        }
        Tab {
            id: rawValues
            visible: false
            title: "raw values"

            property int j: 1

            ColumnLayout {
                id: columnLayout
                anchors.fill: parent

                Text {
                    id: batteryVoltage
                    text: qsTr("Battery Voltage: " + window.battery_voltage)
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
                    text: qsTr("   Left Front: " + window.l_f_drive)
                    font.pixelSize: 12
                }

                Text {
                    id: lmDrive
                    text: qsTr("   Left Middle: " + window.l_m_drive)
                    font.pixelSize: 12
                }

                Text {
                    id: lbDrive
                    text: qsTr("   Left Back: " + window.l_b_drive)
                    font.pixelSize: 12
                }

                Text {
                    id: rfDrive
                    text: qsTr("   Right Front: " + window.r_f_drive)
                    font.pixelSize: 12
                }

                Text {
                    id: rmDrive
                    text: qsTr("   Right Middle: " + window.r_m_drive)
                    font.pixelSize: 12
                }

                Text {
                    id: rbDrive
                    text: qsTr("   Right Back: " + window.r_b_drive)
                    font.pixelSize: 12
                }

                Text {
                    id: swerveDriveState
                    text: qsTr("Swerve Drive State: ")
                    font.pixelSize: 15
                }

                Text {
                    id: armMotors
                    text: qsTr("Arm Motors: ")
                    font.pixelSize: 15
                }
                Text {
                    id: m1
                    text: qsTr("   Motor 1: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: m2
                    text: qsTr("   Motor 2: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: m3
                    text: qsTr("   Motor 3: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: m4
                    text: qsTr("   Motor 4: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: m5
                    text: qsTr("   Motor 5: " + j)
                    font.pixelSize: 12
                }

                Text {
                    id: potentiometers
                    text: qsTr("Potentometers:")
                    font.pixelSize: 15
                }
                Text {
                    id: pot1
                    text: qsTr("   Pot 1: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: pot2
                    text: qsTr("   Pot 2: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: pot3
                    text: qsTr("   Pot 3: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: pot4
                    text: qsTr("   Pot 4: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: pot5
                    text: qsTr("   Pot 5: " + j)
                    font.pixelSize: 12
                }

                Text {
                    id: selectedCamera
                    text: qsTr("Selected Camera: " + j)
                    font.pixelSize: 15
                }

                Text {
                    id: callsign
                    text: qsTr("Callsign: " + j)
                    font.pixelSize: 15
                }

                Text {
                    id: magnetomer
                    text: qsTr("Magnetomer: ")
                    font.pixelSize: 15
                }
                Text {
                    id: magX
                    text: qsTr("   X: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: magY
                    text: qsTr("   Y: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: magZ
                    text: qsTr("   Z: " + j)
                    font.pixelSize: 12
                }


                Text {
                    id: gyroscope
                    text: qsTr("Gyroscope: ")
                    font.pixelSize: 15
                }
                Text {
                    id: gyroX
                    text: qsTr("   X: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: gyroY
                    text: qsTr("   Y: " + j)
                    font.pixelSize: 12
                }
                Text {
                    id: gyroZ
                    text: qsTr("   Z: " + j)
                    font.pixelSize: 12
                }

                Text {
                    id: gpioDir
                    text: qsTr("GPIO direction: " + j)
                    font.pixelSize: 15
                }
                Text {
                    id: gpioOutValue
                    text: qsTr("GPIO out value: " + j)
                    font.pixelSize: 15
                }
                Text {
                    id: gpioReadState
                    text: qsTr("GPIO read state: " + j)
                    font.pixelSize: 15
                }

                Text {
                    id: debugInfo
                    text: qsTr("Debugging info: " + j)
                    font.pixelSize: 15
                }

                Text {
                    id:buildInfo
                    text: qsTr("Build info: " + j)
                    font.pixelSize: 15
                }

            }
        }

    }
}
