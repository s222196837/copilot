import QtQuick 2.15
import QtLocation 5.15

MapQuickItem {
    id: plane
    property var ufo: null
    property string pilot: ""
    property string ico: "local"
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
                source: "qrc:/images/pointer-" + ico + ".png"
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

    function ufoNameChanged() { pilot = ufo.name }
    function ufoStatusChanged() { ico = ufo.status }
    function ufoBearingChanged() { bearing = ufo.bearing }
    function ufoPositionChanged() { coordinate = ufo.coordinate }

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
