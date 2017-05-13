import QtQuick 2.0

Item {
    id: rectangle_color_toggler
    objectName: "rectangle_color_toggler"
    property bool color: false
    signal write
    x: 0
    y: 0
    width: 640
    height: 480
    Rectangle {
        id: rectangle
        color: rectangle_color_toggler.color ? "red" : "blue"
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rectangle_color_toggler.color = !rectangle_color_toggler.color;
                rectangle_color_toggler.write();
            }
        }
    }


}
