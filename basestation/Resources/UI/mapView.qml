import QtQuick 2.0
import QtQuick.Window 2.0

import QtWebEngine 1.4
import QtWebChannel 1.0

import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

WebEngineView {

    id: map
    anchors.fill: parent
    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.minimumWidth: 200
    Layout.preferredWidth: 300
    url: "/home/hodgerti/Documents/OSU_Robotics/Rover2017/basestation/Resources/web/map.html"
    webChannel: WebChannel{
        registeredObjects: [gps]
    }
}
