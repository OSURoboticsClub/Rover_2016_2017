import QtQuick 2.0
import QtQuick.Controls 2.0

import QtQuick.Layouts 1.3

RowLayout{
    id: mainPanel
    StackLayout {
        id: mainStack
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
        id: navColumn
        Layout.minimumWidth: 70
        Layout.fillHeight: true


        NavigationIcon{
            id: nav1
            anchors.left: parent.left
            anchors.right: parent.right
            source: Qt.resolvedUrl("qrc:/img/MWhite.png")
            onClicked: {
                mainStack.currentIndex = 0
                nav1.source = Qt.resolvedUrl("qrc:/img/MWhite.png")
                nav2.source = Qt.resolvedUrl("qrc:/img/SBlack.png")
                nav3.source = Qt.resolvedUrl("qrc:/img/RBlack.png")
            }
        }
        NavigationIcon{
            id: nav2
            anchors.left: parent.left
            anchors.right: parent.right
            source: Qt.resolvedUrl("qrc:/img/SBlack.png")
            onClicked: {
                mainStack.currentIndex = 1
                nav1.source = Qt.resolvedUrl("qrc:/img/MBlack.png")
                nav2.source = Qt.resolvedUrl("qrc:/img/SWhite.png")
                nav3.source = Qt.resolvedUrl("qrc:/img/RBlack.png")            }
        }
        NavigationIcon{
            id: nav3
            anchors.left: parent.left
            anchors.right: parent.right
            source: Qt.resolvedUrl("qrc:/img/RBlack.png")
            onClicked: {
                mainStack.currentIndex = 2
                nav1.source = Qt.resolvedUrl("qrc:/img/MBlack.png")
                nav2.source = Qt.resolvedUrl("qrc:/img/SBlack.png")
                nav3.source = Qt.resolvedUrl("qrc:/img/RWhite.png")
            }
        }
    }
}


