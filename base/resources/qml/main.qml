import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
//import QtWebChannel 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0




Window {
    id: root
    visibility: "Maximized"
    minimumHeight: 300
    minimumWidth: 500

    visible: true

    Material.theme: Material.Dark
    //Material.accent: Material.Purple
    //Material.background: Material.Grey

    signal serialHandlerOn()
    signal serialHandlerOff()
    signal updaterOn()
    signal updaterOff()
    signal controllerHandlerOn()
    signal controllerHandlerOff()



    signal updateRoverPosistion()
    signal roverPosTrigger()

    property var serialPorts: []

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
    property double latitude: 0
    property double longitude: 0


    property int gps_track_valid: 0
    property int gps_heading: 0
    property int gps_speed: 0

    property string colorSerialHandler: "white"
    property bool activeSerialHandler: false
    property string colorControllerHandler: "white"
    property bool activeControllerHandler: false
    property string colorUpdater: "white"
    property bool activeUpdater: false
/*
    QtObject {
        id: gps
        signal pushRoverCoords;
        property var coords: [root.latitude, root.longitude, root.gps_heading]
        //WebChannel.id: "gps"
        onPushRoverCoords: {
            coords = [root.latitude, root.longitude, root.gps_heading];
        }
    }*/
    // TODO: is this actually necessary?
    // Connor: Not sure. I'm sure that there is a better way, but this got the job done when I was putting it in.
    /*
    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: {
            gps.pushRoverCoords();
        }
    }
    */
    Pane {
        anchors.fill: parent
        RowLayout {
            id: rowLayout
            anchors.fill: parent

            SidebarPanel {
                Layout.margins: 2
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.preferredWidth: 200
                Layout.maximumWidth: 200
            }


            ColumnLayout {
                id: mainPanelRow
                Layout.fillHeight: true
                Layout.fillWidth: true
   //             Layout.minimumWidth: 400
     //           Layout.minimumHeight: 500

                MainPanel {
                    Layout.minimumHeight: 350
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                TextArea {
                    id: logger
                    Layout.fillWidth: true
                    Layout.minimumHeight: 150
                    Layout.preferredHeight: 150
                }
            }


        }
    }
}

