import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15
import QtLocation 5.15

Page {
    height: 700
    width: 500
    visible: true
    title: "COPILOT"

    property variant torquay: QtPositioning.coordinate(-38.342647, 144.319029)

    property variant viewPoint: torquay
    property variant viewPort: QtPositioning.rectangle(viewPoint, 0.008, 0.08)

    Map {
        id: gpsMap
        anchors.centerIn: parent;
        anchors.fill: parent
        plugin: Plugin {
            name: "mapboxgl" // TODO: optional - "osm", "mapboxgl", "esri", ...
        }

        Plane {
            id: gpsPlane
            // pilotName: "GPS"
            coordinate: gps.position

            SequentialAnimation {
                id: gpsPlaneAnimation
                property real rotationDirection : 0;
                NumberAnimation {
                    target: gpsPlane; property: "bearing"; duration: 100
                    easing.type: Easing.InOutQuad
                    to: gpsPlaneAnimation.rotationDirection
                }
            }
	}

        visibleRegion: viewPort
    }
}
