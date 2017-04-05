import QtQuick 2.0
import QtQuick.Controls 2.0

import QtQuick.Layouts 1.3



RowLayout{

    StackLayout {
        id: mainPanel
        Layout.fillHeight: true
        Layout.fillWidth: true
        currentIndex: 0
        MapView{anchors.fill: parent}

        SettingsView{anchors.fill: parent}
        DataView{anchors.fill: parent}

    }

    ListView {
        id: listView
        Layout.minimumWidth: 70
        Layout.fillHeight: true

        delegate: Row {
                id: row1
                spacing: 10
                Image {
                    source: Qt.resolvedUrl("qrc:/img/placeholder-icon.svg")
                    height: 40
                    width: 40
                    fillMode: Image.PreserveAspectFit
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
        model: ["map", "settings", "data"]
    }
}

