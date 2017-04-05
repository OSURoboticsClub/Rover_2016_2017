import QtQuick 2.0

import QtWebEngine 1.3
import QtWebChannel 1.0

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

ColumnLayout {

    WebEngineView {
        id: map

        Layout.fillHeight: true
        Layout.fillWidth: true

        url: "qrc:/web/map.html"
        webChannel: WebChannel {
            registeredObjects: [gps]
        }
    }

    Row {
        Layout.fillWidth: true
        Layout.minimumHeight: 70
        spacing: 10
        TextField {
            placeholderText: "latitude"
            validator: DoubleValidator{}
        }
        TextField {
            placeholderText: "longitude"
            validator: DoubleValidator{}
        }
        TextField {
            placeholderText: "name"
        }
        Button {
            text: "Add Waypoint"
        }
    }
}
