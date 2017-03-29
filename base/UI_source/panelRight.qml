import QtQuick 2.0
import QtQuick.Controls 2.0
import QtPositioning 5.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import QtQuick.Templates 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls 1.0
import QtQuick.Controls 1.4

Item {
    id: panelRight
    anchors.fill: parent
    //shamelessly copied from Ravindranath Akila on stack overflow. This is the haversine formula
    function latLngDist(lat1, lon1, lat2, lon2){  // generally used geo measurement function
        var R = 6378.137; // Radius of earth in KM
        var dLat = lat2 * Math.PI / 180 - lat1 * Math.PI / 180;
        var dLon = lon2 * Math.PI / 180 - lon1 * Math.PI / 180;
        var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
                Math.cos(lat1 * Math.PI / 180) * Math.cos(lat2 * Math.PI / 180) *
                Math.sin(dLon/2) * Math.sin(dLon/2);
        var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
        var d = R * c;
        return d * 1000; // meters
    }

    Item {
        id: timerBackwards
        x: 0
        width: 200
        height: 200
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Loader {
            id: thisTimer
            anchors.fill: parent
            source: "countDownTimer.qml"
        }
    }

    Text {
        id: lat
        text: "Lat: [" + root.latitude + "]";
        anchors.left: parent.left
        anchors.leftMargin: 0
        font.pointSize: 16
        style: Text.Normal
        anchors.top: timerBackwards.bottom
        anchors.topMargin: 25

    }
    Text {
        id: lng
        text: "Lng: [" + root.longitude + "]";
        style: Text.Normal
        font.pointSize: 16
        font.family: "Tahoma"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: lat.bottom
        anchors.topMargin: 10

    }

    TextInput {
        id: lat1
        x: 136
        y: 308
        width: 80
        height: 20
        text: qsTr("lat1")
        anchors.horizontalCenter: lng.horizontalCenter
        font.pixelSize: 16
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                lat1.text = "";
//            }
//        }
    }

    TextInput {
        id: lng1
        y: 308
        width: 80
        height: 20
        text: qsTr("lng1")
        anchors.left: lat1.right
        anchors.leftMargin: 22
        font.pixelSize: 16
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                lng1.text = "";
//            }
//        }
    }

    TextInput {
        id: lat2
        x: 136
        y: 343
        width: 80
        height: 20
        text: qsTr("lat2")
        anchors.horizontalCenter: lat1.horizontalCenter
        font.pixelSize: 16
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                lat2.text = "";
//            }
//        }
    }

    TextInput {
        id: lng2
        x: 238
        y: 343
        width: 80
        height: 20
        text: qsTr("lng2")
        anchors.horizontalCenter: lng1.horizontalCenter
        font.pixelSize: 16
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                lng2.text = "";
//            }
//        }
    }

    Text {
        id: distOutput
        text: "Output"
        anchors.top: parent.top
        anchors.topMargin: 377
        anchors.left: lat2.right
        anchors.leftMargin: -80
        font.pixelSize: 12
    }


    Text {
        function isPaused(){
            if (root.frSkyPaused){
                return "PAUSED"
            }
            else {
                return "RUNNING"
            }
        }

        id: frSkyPaused
        x: 307
        text: frSkyPaused.isPaused()
        anchors.top: lat2.bottom
        anchors.topMargin: 85
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
    }

    Button {
        id: calcDist
        x: 30
        y: 308
        text: qsTr("Calc Dist")
        anchors.top: distOutput.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 50
        onClicked: {
            distOutput.text = "Dist: " + panelRight.latLngDist(lat1.text,lng1.text,lat2.text,lng2.text) + " m";
        }
    }

}
