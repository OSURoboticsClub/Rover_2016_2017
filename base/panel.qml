import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: item1
    Button {
        id: exampleButton
        x: 253
        y: 197
        text: qsTr("Example Button")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

}
