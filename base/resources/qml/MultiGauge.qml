import QtQuick 2.0
import QtQuick.Layouts 1.3

RowLayout {
    id: multiGauge
    height: 40
    width: 100
    property alias model: repeater.model

    property var limits
    property string bgcolor: "white"
    property string fgcolor: "orange"


    QtObject {
        id: p
        property int range: Math.abs(multiGauge.limits[0] -
                                     multiGauge.limits[1])
        property int centerY: Math.abs(multiGauge.limits[0] +
                                       multiGauge.limits[1]) / 2
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
                border.width: 1
            }

            Rectangle {
                color: multiGauge.fgcolor
                width: parent.width
                border.width: 1
                // TODO: while this works, it is not very clean
                // Could this be made better?


                height: multiGauge.height * Math.abs(model.modelData) / p.range

                y: model.modelData > 0 ? (0.5 * p.range - model.modelData + p.centerY) *
                                         multiGauge.height / p.range :
                                         0.5 * p.height

            }

            Rectangle {
                width: parent.width
                height: 1
                y: (p.centerY + 0.5 * p.range)* (multiGauge.height) / p.range
                color: "black"

                Component.onCompleted: {
                    console.log(parent.width);
                }
            }

        }
    }
}
