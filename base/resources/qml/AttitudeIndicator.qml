/*
  Adapted from QFlightInstruments
  */


import QtQuick 2.0

Canvas {
    id: attitudeIndicator
    property double roll: 0
    property double pitch: 0

    Image {
        id: image
        anchors.fill: parent
        source: Qt.resolvedUrl("qrc:/img/adi/adi_back.svg")
        transformOrigin: Item.Center
        fillMode: Image.PreserveAspectFit
        rotation: -attitudeIndicator.roll
    }
    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("qrc:/img/adi/adi_face.svg")
        transformOrigin: Item.Center
        rotation: -attitudeIndicator.roll
        fillMode: Image.PreserveAspectFit
    }

    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("qrc:/img/adi/adi_ring.svg")
        transformOrigin: Item.Center
        fillMode: Image.PreserveAspectFit
        rotation: -attitudeIndicator.roll
    }
    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("qrc:/img/adi/adi_case.svg")
        fillMode: Image.PreserveAspectFit
    }

    function faceTranslation() {
        var rollRad = Math.PI * attitudeIndicator.roll / 180.0
        var delta = 1.7 * attitudeIndicator.pitch
        var dx = delta * Math.sin(rollRad)
        var dy = delta * Math.cos(rollRad)
        return([dx, dy])
    }
}
