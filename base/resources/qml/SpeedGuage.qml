import QtQuick 2.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.0


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
                color: Material.accent
                antialiasing: true
            }
            foreground: Item {
                Rectangle {
                    width: outerRadius * 0.2
                    height: width
                    radius: width / 2
                    color: Material.color(Material.Red)
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
