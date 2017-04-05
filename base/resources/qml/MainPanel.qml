import QtQuick 2.0
import QtQuick.Controls 2.0

import QtQuick.Layouts 1.3

import QtWebEngine 1.3
import QtWebChannel 1.0

RowLayout{
    StackLayout {
        id: mainPanel
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.fill: parent
        currentIndex: 0


        WebEngineView {
            id: map
            anchors.fill: parent
            url: "qrc:/web/map.html"
            webChannel: WebChannel {
                registeredObjects: [gps]
            }
        }


        SettingsView{anchors.fill: parent}

        DataView{anchors.fill: parent}
    }

    ListView {
        id: listView
        Layout.minimumWidth: 70
        Layout.fillHeight: true
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row1
                spacing: 10
                Rectangle {
                    width: 40
                    height: 40
                    color: "blue"
                    Text {
                        text: model.modelData
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log(model.index)
                            mainPanel.currentIndex = model.index
                    }
                }
            }
        }
        }
        model: ["map", "settings", "data"]
        }
    }

