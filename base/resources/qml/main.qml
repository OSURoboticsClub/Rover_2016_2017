import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0



Window {
    id: root

    visibility: "Maximized"
    minimumHeight: 500
    minimumWidth: 800

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
    signal pauseAllThreads()
    signal resumeAllThreads()

    /*
    signal updateRoverPosistion()
    signal roverPosTrigger()
    */
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



    //TODO: instead of gps heading also add root function that calculates heading using magnometer and accelometer
    function heading () {
        return Math.tan2(root.mag_x, root.mag.y);
    }

    Pane {
        id: pane
        anchors.fill: parent
        RowLayout {
            id: mainLayout
            anchors.fill: parent

            SidebarPanel {
                id: sidebarPanel
                Layout.margins: 2
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.preferredWidth: 200
                Layout.maximumWidth: 200
            }

            ColumnLayout {
                id: mainPanelLayout
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 600
                Layout.minimumHeight: 400

                MainPanel {
                    id: mainPanel
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

