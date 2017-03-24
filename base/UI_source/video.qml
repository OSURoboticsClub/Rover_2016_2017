import QtQuick 2.0


Item {
    id: videoView
    width: parent.width
    height: parent.height
    property int widthSpinStart: 75
    property int heightSpinStart: 75
    property int draggerStartX: 146
    property int draggerStartY: 198
    property string draggerColor: "red"
    property int modWidth: 0
    property int modHeight: 0

    //    Rectangle {
    //        x: 247
    //        y: 527
    //        width: widthSpinStart + (.2 * dragger.x); height: heightSpinStart + (.2 * dragger.y); color: "green";
    //        RotationAnimation on rotation {
    //            loops: Animation.Infinite
    //            from: 0
    //            to: 360
    //        }
    //    }


    Rectangle {
        x: draggerStartX; y: draggerStartY;
        width: 50; height: 50
        color: draggerColor
        id: dragger

        // I've added this property for simplicity's sake.
        property bool dragActive: dragArea.drag.active

        // This can be used to get event info for drag starts and
        // stops instead of onDragStarted/onDragFinished, since
        // those will neer be called if we don't use Drag.active
        onDragActiveChanged: {

            if (dragActive) {
                videoView.draggerColor = "red";
            } else {
                videoView.draggerColor = "red";
                modWidth = modWidth + (dragger.x - draggerStartX);
                modHeight = modHeight + (dragger.y - draggerStartY);
                dragger.x = draggerStartX
                dragger.y = draggerStartY
            }



        }

        Drag.dragType: Drag.Automatic


        MouseArea {
            id: dragArea
            width: 50
            height: 50
            x: videoView.draggerStartX; y: videoView.draggerStartY;
            anchors.fill: parent
            drag.target: parent
        }
    }

    Rectangle {
        id: right
        x: 232
        y: 175
        width: widthSpinStart + (.2 * (dragger.x - videoView.draggerStartX + modWidth))
        height: heightSpinStart + (.2 * (dragger.y - videoView.draggerStartY + modHeight))
        color: videoView.draggerColor
    }

    Rectangle {
        id: left
        x: 39
        y: 175
        width: widthSpinStart + (.2 * (dragger.x - videoView.draggerStartX + modWidth))
        height: heightSpinStart + (.2 * (dragger.y - videoView.draggerStartY + modHeight))
        color: videoView.draggerColor
    }

    Rectangle {
        id: bottom
        x: 140
        y: 280
        width: widthSpinStart + (.2 * (dragger.x - videoView.draggerStartX + modWidth))
        height: heightSpinStart + (.2 * (dragger.y - videoView.draggerStartY + modHeight))
        color: videoView.draggerColor
    }

    Rectangle {
        x: 131
        y: 74
        width: widthSpinStart + (.2 * (dragger.x - videoView.draggerStartX + modWidth))
        height: heightSpinStart + (.2 * (dragger.y - videoView.draggerStartY + modHeight))
        color: videoView.draggerColor

    }

}
