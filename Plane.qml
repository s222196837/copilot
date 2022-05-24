import QtQuick 2.15
import QtLocation 5.15

MapQuickItem {
    id: plane
    property variant ufo
    property string avatar: "pointer"
    property string pilot: ""
    property int bearing: 0

    anchorPoint.x: image.width/2
    anchorPoint.y: image.height/2

    sourceItem: Grid {
        columns: 1
        Grid {
            horizontalItemAlignment: Grid.AlignHCenter
            Image {
                id: image
                rotation: bearing
                source: "qrc:/images/" + avatar + ".png"
            }
            Rectangle {
                id: bubble
                color: "lightblue"
                border.width: 1
                width: text.width * 1.3
                height: text.height * 1.3
                radius: 5
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: pilot
                }
            }
        }
    }

    SequentialAnimation {
        id: planeAnimation
        property real rotationDirection : 0;
        NumberAnimation {
            target: plane; property: "bearing"; duration: 100
            easing.type: Easing.InOutQuad
            to: planeAnimation.rotationDirection
        }
    }
}
