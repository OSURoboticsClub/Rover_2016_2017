import QtQuick 2.0

import QtWebEngine 1.3
import QtWebChannel 1.0

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0


ColumnLayout {
    id: mapView
    //anchors.margins: 10
    property int largeFontSize: 25
    property int smallFontSize: 20
    property string fontColor: "white"

    WebEngineView {
        id: map

        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.minimumWidth: 200
        Layout.preferredWidth: 300
        url: "qrc:/web/map.html"
        webChannel: WebChannel{
            registeredObjects: [gps]
        }


    }
    Row {
        id: mapInput
        Layout.fillWidth: true
        Layout.minimumHeight: 50
        Layout.preferredHeight: 50
        spacing: 10
        TextField {
            id: inputLatitude
            placeholderText: "latitude"
            validator: DoubleValidator{}
        }
        TextField {
            id: inputLongitude
            placeholderText: "longitude"
            validator: DoubleValidator{}
        }
        TextField {
            id: inputName
            placeholderText: "name"
        }
        Button {
            id: submitButton
            text: "Add Waypoint"
            onClicked: {
                gps._submitButton();
            }
        }
        Button {
            id: addFromClicked
            text: "Add From CLicked"
            onClicked: {
                gps._addFromClicked();
            }
        }

        Button {
            id: deleteClicked
            text: "Delete Clicked"
            onClicked: {
                gps._deleteClicked();
            }
        }
        Button {
            id: deleteAll
            text: "Delete All"
            onClicked: {
                gps._deleteAll();
            }
        }
        QtObject {
            id: gps
            objectName: "gps"
            WebChannel.id: "gps"

            property double _inputLatitude: parseFloat(inputLatitude.text)
            property double _inputLongitude: parseFloat(inputLongitude.text)
            property string _inputName: inputName.text
            property var coords: [root.latitude, root.longitude, root.gps_heading]

            property string dist: "0 m"
            property string name: ""

            signal _pushRoverCoords()
            signal _submitButton()
            signal _addFromClicked()
            signal _deleteClicked()
            signal _deleteAll()

            function  roverPosChangeTrigger() {
                gps.coords = [root.latitude, root.longitude, root.heading];
                _pushRoverCoords();
            }
        }
    }
    function checkNull(val, text) {
        if (val)
            return val
        else
            return text
    }

    Row {
        id: mapOutputName
        Layout.fillWidth: true
        Layout.minimumHeight: 50
        Layout.preferredHeight: 50
        spacing: 10
        anchors.top: mapInput.bottom
        anchors.topMargin: 15
        Text {
            id: name
            text: "Name: " + gps.name
            font.pixelSize: mapView.largeFontSize
            color: mapView.fontColor
        }
    }
    Row {
        id: mapOutputDist
        Layout.fillWidth: true
        Layout.minimumHeight: 50
        Layout.preferredHeight: 50
        spacing: 10
        anchors.top: mapOutputName.bottom
        anchors.topMargin: 15
        Text {
            id: dist
            text: "Distance: " + gps.dist
            font.pixelSize: mapView.largeFontSize
            color: mapView.fontColor
        }
    }
}


