import QtQuick 2.0
import QtQuick.Controls 1.4
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

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                ColumnLayout {
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Text {
                        id: batteryVoltage
                        text: qsTr("Battery Voltage: " + j)
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
                        text: qsTr("   Left Front: " + j)
                        font.pixelSize: 12
                    }

                    Text {
                        id: lmDrive
                        text: qsTr("   Left Middle: " + j)
                        font.pixelSize: 12
                    }

                    Text {
                        id: lbDrive
                        text: qsTr("   Left Back: " + j)
                        font.pixelSize: 12
                    }

                    Text {
                        id: rfDrive
                        text: qsTr("   Right Front: " + j)
                        font.pixelSize: 12
                    }

                    Text {
                        id: rmDrive
                        text: qsTr("   Right Middle: " + j)
                        font.pixelSize: 12
                    }

                    Text {
                        id: rbDrive
                        text: qsTr("   Right Back: " + j)
                        font.pixelSize: 12
                    }

                    Text {
                        id: swerveDriveState
                        text: qsTr("Swerve Drive State: " + j)
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
                }
                ColumnLayout {
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    anchors.top: mainTab.bottom
                    anchors.topMargin: -240
                    anchors.bottom: mainTab.top
                    anchors.bottomMargin: -190
                    anchors.left: mainTab.right
                    anchors.leftMargin: -624
                    anchors.right: mainTab.left
                    anchors.rightMargin: -138
                    anchors.horizontalCenter: mainTab.horizontalCenter
                    anchors.verticalCenter: mainTab.verticalCenter
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
}
