import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Item {
    id: root
    width: 800
    height: 600

    property int batteryVoltage: 0

    signal _serialHandlerOn()
    property int battery_voltage: 0
    property int l_f_drive: 0
    property int l_m_drive: 0
    property int l_b_drive: 0
    property int r_f_drive: 0
    property int r_m_drive: 0
    property int r_b_drive: 0
        Column {
            id: sidebarCol
            width: 0.3 * parent.width
            height: parent.height
            Loader {
                id: sidebarLoader
                source: "panel.qml"
            }
        }

        Column {
            id: mainCol
            anchors.left: sidebarCol.right
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            height: parent.height

            Row {
                id: contentRow
                width: parent.width
                height: parent.height * 0.8
                Loader {
                    id: contentLoader
                    width: parent.width
                    height: parent.height
                    source: "mainview.qml"
                }
            }

            Row {
                id: consoleRow
                width: parent.width
                height: parent.height * 0.2
                TextArea {
                    id: logger
                    width: parent.width
                    height: parent.height
                    readOnly: true
                }
            }
        }
    }

