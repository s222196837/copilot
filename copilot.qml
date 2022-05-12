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

            MouseArea {
                anchors.fill: parent
                onClicked: { viewPointGPS = true; } // reset after pan/zoom
	    }

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

        Component.onCompleted: {
            gps.positionChanged.connect(positionChanged)
        }

        function positionChanged () {
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
