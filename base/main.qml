import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtWebChannel 1.0
import QtQuick.Controls 1.4

Item {
    id: root
    width: 800
    height: 600



    signal _serialHandlerOn()
    signal _serialHandlerOff()
    signal _updaterOn()
    signal _updaterOff()
    signal _controllerHandlerOn()
    signal _controllerHandlerOff()
    signal _allThreadsClose()
    signal _pauseAllThreads()
    signal _resumeAllThreads()


    property int battery_voltage: 0
    property int l_f_drive: 0
    property int l_m_drive: 0
    property int l_b_drive: 0
    property int r_f_drive: 0
    property int r_m_drive: 0
    property int r_b_drive: 0
    property int swerve_state: 0
    property int arm_motor_1: 0
    property int arm_motor_2: 0
    property int arm_motor_3: 0
    property int arm_motor_4: 0
    property int arm_motor_5: 0
    property int selected_camera: 0
    property int pot_1: 0
    property int pot_2: 0
    property int pot_3: 0
    property int pot_4: 0
    property int pot_5: 0
    property int mag_x: 0
    property int mag_y: 0
    property int mag_z: 0
    property string callsign_data: "NULL"
    property int gyro_x: 0
    property int gyro_y: 0
    property int gyro_z: 0
    property int gpio_dir: 0
    property int gpio_out: 0
    property int gpio_state: 0
    property string debug_str_data: "NULL"
    property string build_info_data: "NULL"
    property int gps_pos_valid: 0
    property double altitude: 0
    property double latitude: 0 //38.4063
    property double longitude: 0 //-110.792286
    property int gps_track_valid: 0
    property int gps_heading: 0
    property int gps_speed: 0

    property string colorSerialHandler: "white"
    property bool activeSeriaHandler: false
    property string colorControllerHandler: "white"
    property bool activeControllerHandler: false
    property string colorUpdater: "white"
    property bool activeUpdater: false


    QtObject {
        id: gps
        property var coords: [root.latitude, root.longitude]
        WebChannel.id: "gps"
    }

        Column {
            id: sidebarCol
            width: 0.3 * parent.width
            height: parent.height
            Loader {
                id: sidebarLoader
                anchors.fill: parent
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

