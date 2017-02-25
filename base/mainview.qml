import QtQuick 2.0
import QtQuick.Controls 1.4
//import QtWebEngine 1.4
import QtWebEngine 1.3
import QtQuick.Layouts 1.3

Item {
    id: mainView
    //width: parent.width
    //height: parent.height
    property int j: 1
    TabView {
        id: mainTab
        width: parent.width
        height: parent.height
        //        anchors.fill: parent
        Tab {
            id: map
            title: "map"

            WebEngineView {
                anchors.fill: parent
                url: "map.html"
            }
            /*           Item {
                id: map

            }
*/
        }
        Tab{
            id: webGame
            title: "Webgame"

            WebEngineView {
                anchors.fill: parent
                url: "superGame.html"
            }
        }

        Tab {
            id: settings
            title: "Settings"
            source: "Settings.qml"
        }
        Tab {
            id: rawValues
            visible: false
            title: "raw values"
            source: "rawValues.qml"

        }

    }
}
