import QtQuick 2.0
import QtQuick.Layouts 1.3

RowLayout {
    id: multiGauge
    height: 40
    width: 100
    property alias model: repeater.model

    property var limits: [-127, 127]
    property string bgcolor: "#ee8948"
    property string fgcolor: "#5073ef"

    Repeater {
        id: repeater
        Item {
            id: gauge
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: multiGauge.bgcolor
                anchors.fill: parent
                border.width: 2
            }
            Rectangle {
                color: multiGauge.fgcolor
                width: parent.width

                property int range: Math.abs(multiGauge.limits[0] -
                                             multiGauge.limits[1])

                height: multiGauge.height * Math.abs(model.modelData) / range

                y: model.modelData > 0 ? (0.5 * range - model.modelData) *
                                         multiGauge.height / range :
                                         0.5 * multiGauge.height
            }
        }
    }
}
