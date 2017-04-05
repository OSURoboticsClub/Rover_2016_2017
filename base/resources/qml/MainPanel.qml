import QtQuick 2.0
import QtQuick.Controls 2.0

import QtQuick.Layouts 1.3

RowLayout{

    StackLayout {
        id: mainPanel
        Layout.fillHeight: true
        Layout.fillWidth: true
        currentIndex: 0

        MapView{
            id: mapView
            anchors.fill: parent
        }

        SettingsView{
            id: settingsView
            anchors.fill: parent
        }
        DataView{
            id: dataView
            anchors.fill: parent
        }

    }

    Column {
        id: listView
        Layout.minimumWidth: 70
        Layout.fillHeight: true

        NavigationIcon{
            anchors.left: parent.left
            anchors.right: parent.right
            source: Qt.resolvedUrl("qrc:/img/placeholder-icon.svg")
            onClicked: {
                mainPanel.currentIndex = 0
            }
        }
        NavigationIcon{
            anchors.left: parent.left
            anchors.right: parent.right
            source: Qt.resolvedUrl("qrc:/img/placeholder-icon.svg")
            onClicked: {
                mainPanel.currentIndex = 1
            }
        }
        NavigationIcon{
            anchors.left: parent.left
            anchors.right: parent.right
            source: Qt.resolvedUrl("qrc:/img/placeholder-icon.svg")
            onClicked: {
                mainPanel.currentIndex = 2
            }
        }
    }
}


