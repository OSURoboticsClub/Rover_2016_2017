import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

RowLayout {
    id: multiGauge
    height: 40
    width: 100
    property alias model: repeater.model

    property var limits
    property string bgcolor: Material.foreground
    property string fgcolor: Material.accent


    QtObject {
        id: p
        property int range: multiGauge.limits[1] - multiGauge.limits[0]
        property int centerY: multiGauge.limits[1] * multiGauge.height / p.range
    }

    Repeater {
        id: repeater
        Item {
            id: gauge
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: multiGauge.bgcolor
                anchors.fill: parent
            }

            Rectangle {
                color: multiGauge.fgcolor
                anchors.left: parent.left
                anchors.right: parent.right

                height: multiGauge.height * Math.abs(model.modelData) / p.range

                y: model.modelData > 0 ? p.centerY - height :
                                         p.centerY
            }

            Rectangle {
                width: parent.width
                height: 2
                y: p.centerY
                color: Material.background


            }

        }
    }
}
