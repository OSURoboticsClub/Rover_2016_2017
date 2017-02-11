import QtQuick 2.0
import QtQuick.Controls 1.4
//import QtWebEngine 1.3
import QtQuick.Layouts 1.3

Item {
    id: mainView
    width: parent.width
    height: parent.height
    property int j: 1
    TabView {
        id: mainTab
        width: parent.width
        height: parent.height
//        anchors.fill: parent
        Tab {
            title: "map"
 /*           Item {
                id: map
                WebEngineView {
                    anchors.fill: parent
                    url: "map.html"
                }
            }
*/
        }
        Tab {
            title: "raw values"

            ColumnLayout {
                id: columnLayout
                anchors.fill: parent
            }
        }
    }
}
