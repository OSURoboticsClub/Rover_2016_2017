import QtQuick 2.0
import QtQuick.Controls 1.4

import QtQuick.Layouts 1.3

import QtWebEngine 1.3
import QtWebChannel 1.0


Item {
    id: mainPanel

    TabView {
        id: mainTabView
        anchors.fill: parent

        Tab {
            id: mapTab
            title: "Map"

            WebEngineView {
                id: map
                anchors.fill: parent
                url: "qrc:/web/map.html"
                webChannel: WebChannel {
                    registeredObjects: [gps]
                }
            }
        }
        Tab {
            id: settingsTab
            title: "Settings"
            source: "qrc:/qml/SettingsView.qml"
        }
        Tab {
            id: dataTab
            title: "Data"
            source: "qrc:/qml/DataView.qml"

        }

    }
}

