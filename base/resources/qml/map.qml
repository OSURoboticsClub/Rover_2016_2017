import QtQuick 2.0


Item {
    id: map
    WebEngineView {
        anchors.fill: parent
        url: "map.html"
    }
}
