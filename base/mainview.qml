import QtQuick 2.0
import QtQuick.Controls 1.4
import QtWebEngine 1.3
//import QtWebEngine 1.3
import QtQuick.Layouts 1.3
import QtWebChannel 1.0

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
            id: mapTab
            title: "map"

            WebEngineView {
                id: map
                anchors.fill: parent
                url: "map.html"
                webChannel: WebChannel{
                    //WebChannel.id: "channel"
                    registeredObjects: [gps]
                }
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
