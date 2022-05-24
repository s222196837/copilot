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

    property variant viewPointGPS: true
    property variant viewPoint: torquay
    property variant viewPort: QtPositioning.rectangle(viewPoint, 0.008, 0.08)
    property variant ufoIndex: 0        // between 0-3, used for avatar

    Map {
        id: gpsMap
        anchors.centerIn: parent;
        anchors.fill: parent
        plugin: Plugin {
            name: "mapboxgl" // TODO: optional - "osm", "mapboxgl", "esri", ...
        }

        Plane {
            id: gpsPlane
            coordinate: gps.position

            MouseArea {
                anchors.fill: parent
                onClicked: { viewPointGPS = true; } // reset after pan/zoom
            }
        }

        Component.onCompleted: {
            altimu10.yawChanged.connect(bearingChanged)
            gps.positionChanged.connect(positionChanged)
            gaggle.objectAppeared.connect(objectAppeared)
        }

        function objectAppeared(ufo) {
            var component1 = Qt.createComponent("Plane.qml", gpsMap)
            var plane = component1.createObject(gpsMap)
            plane.ufo = ufo
            plane.pilot = ufo.name
            plane.bearing = ufo.bearing
            plane.coordinate = ufo.coordinate
            plane.avatar = 'pointer' + ufoIndex
            ufoIndex = ufoIndex % 4; // prepare next avatar
            gpsMap.addMapItem(plane)
            ufo.bearingChanged.connect(ufoBearingChanged, plane)
            ufo.positionChanged.connect(ufoPositionChanged, plane)
        }

        function ufoBearingChanged(plane, ufo) {
            plane.bearing = ufo.yaw
        }
        function ufoPositionChanged(plane, ufo) {
            plane.position = ufo.position
        }

        function bearingChanged() {
            // update the heading irrespective of viewPointGPS
            gpsPlane.bearing = altimu10.yaw
        }
        function positionChanged() {
            if (viewPointGPS) {
                viewPoint = gps.position;
                var hOffset = gpsMap.zoomLevel / 1000.0;
                var vOffset = gpsMap.zoomLevel / 100.0;
                viewPoint.latitude -= (vOffset * 0.2);
                viewPort = QtPositioning.rectangle(viewPoint, hOffset, vOffset);
            }
        }

        // block updates from GPS during interaction
        gesture.onPanStarted: { viewPointGPS = false; }
        gesture.onTiltStarted: { viewPointGPS = false; }
        gesture.onFlickStarted: { viewPointGPS = false; }
        gesture.onPinchStarted: { viewPointGPS = false; }
        gesture.onRotationStarted: { viewPointGPS = false; }

        visibleRegion: viewPort
    }
}
