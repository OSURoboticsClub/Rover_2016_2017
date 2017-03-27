import QtQuick 2.0

Item {
    id: voltageGuage
    property int min: 20
    property int max: 25
    property double value: testVoltProgressBar
    property color color: "green"

    width: parent.width; height: 23
    //        clip: true
    Rectangle {
        id: border
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

        width: (parent.width) * ((voltageGuage.value - voltageGuage.min)/(voltageGuage.max - voltageGuage.min));
        color: "red"

    }
}
