import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import QtQuick.Controls 1.4
import QtPositioning 5.7


ColumnLayout {
    id: sidebarPanel
    anchors.fill: parent

    Item {
        id: item1
        width: 200
        height: 200
        Layout.fillWidth: true

        Text {
            id: text1
            text: "attitude indicator"
            font.pixelSize: 12
        }
    }

    CircularGauge {
        id: circularGauge
        Layout.minimumWidth: 0
        Layout.preferredHeight: 200
        Layout.minimumHeight: 150
        Layout.fillWidth: true
    }


    Item {
        id: item2
        width: 200
        height: 200
        Layout.fillWidth: true

        Text {
            id: text2
            text: "arm motors and driver motor power"
            font.pixelSize: 12
        }
    }

    Item {
        id: item3
        width: 200
        height: 200
        Layout.fillWidth: true

        Text {
            id: text3
            text: "battery voltage, selected camera, and swerve drive state"
            anchors.fill: parent
            elide: Text.ElideNone
            renderType: Text.QtRendering
            fontSizeMode: Text.HorizontalFit
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }
    }

}

