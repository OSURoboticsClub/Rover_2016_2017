import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtPositioning 5.7
import QtQuick.Controls.Material 2.0





//Rectangle {
//    property int limit: 25
//    property double value: 20000 / 1000
//    Layout.fillWidth: true
//    Layout.minimumHeight: 40

//    color: Material.foreground

//    Rectangle {
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        width: parent.value * parent.width / parent.limit
//        color: parent.width/2 > width ?
//                   Material.color(Material.Red):
//                   Material.accent
//    }

//    Label {
//        text: parent.value + " V"
//        anchors.verticalCenter: parent.verticalCenter
//        anchors.horizontalCenter: parent.horizontalCenter
//    }

//}
Rectangle {
    property int min: 20
    property int max: 25
    property double value: 21
    property color color: Material.foreground
    property double fakeVoltage: (parent.width) * ((voltageGuage.value - voltageGuage.min)/(voltageGuage.max - voltageGuage.min));


    width: parent.width; height: 37
    Rectangle {
        id: border
        height: 51
        anchors.fill: parent
        anchors.bottomMargin: 2
        anchors.rightMargin: 2
        color: "transparent"
        border.width: 2
        border.color: parent.color
    }
    Rectangle {
        id: progress
        anchors.top: border.top
        anchors.bottom: border.bottom
        anchors.left: border.left
        anchors.margins: 2
        width: parent.fakeVoltage
        color: ((voltageGuageLabel.width/2) + (parent.width/2)) <= (progress.width) ? Material.color(Material.Red) : Material.accent
    }

    Text {
        id: voltageGuageLabel
        x: 71
        y: 10
        text: parent.value
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.family: "Tahoma"
        font.pixelSize: 13
        color: ((voltageGuageLabel.width/2) + (parent.width/2)) <= (progress.width) ? "white" : "blue";
    }
}

