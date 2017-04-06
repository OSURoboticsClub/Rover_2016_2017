import QtQuick 2.0

import QtWebEngine 1.3
import QtWebChannel 1.0

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0


ColumnLayout {
    id: mapView
        //anchors.margins: 10


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

            property double _inputLatitude: 38.40626//parseFloat(inputLatitude.text)
            property double _inputLongitude: -110.79299//parseFloat(inputLongitude.text)
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
                gps.coords = [root.latitude, root.longitude, root.gps_heading];
                _pushRoverCoords();
            }
        }
        Text {
            id: name
            text: gps.name
        }
        Text {
            id: dist
            text: gps.dist
        }
    }
}


