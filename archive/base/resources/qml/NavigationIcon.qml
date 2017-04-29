import QtQuick 2.0

import QtQuick.Controls.Material 2.0


Image {
    id: navigationIcon
    fillMode: Image.PreserveAspectFit

    signal clicked()

    Rectangle {
        id: leftRect
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        color: Material.color(Material.Grey)
        width: 2
    }

    Rectangle {
        id: rightRect
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        color: Material.color(Material.Grey)
        width: 2
    }

    MouseArea {
        id: area
        anchors.fill: parent
        onClicked: {
            // TODO: states

            /*
            leftRect.color = Material.accent
            rightRect.color = Material.accent
            */

            parent.clicked()

        }
    }

}
