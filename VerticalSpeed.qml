import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var climbRate: 0.0
    property var lastAltitude: 0.0
    property var lastTimestamp: 0.0
    property var climbRateAngle: 0.0

    Button {
	background: Rectangle {
	    id: background
	    color: baseColor
	    anchors.fill: parent

	    Image {
		id: vsi_case
		source: "qrc:/images/vsi/vsi_case.svg"
		height: length
		width: length
		z: 10
	    }

	    Image {
		id: vsi_face
		source: "qrc:/images/vsi/vsi_face.svg"
		height: length
		width: length
		z: -20
	    }

	    Image {
		id: vsi_hand
		source: "qrc:/images/vsi/vsi_hand.svg"
		height: length
		width: length
		z: -10

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: climbRateAngle
		}
	    }
	}

        Component.onCompleted: {
            gps.positionChanged.connect(updateAltitude);
        }

        function updateAltitude() {
	    var timestamp = gps.timestamp.getUTCMilliseconds() / 1000.0;
	    var altitude = gps.position.altitude * 3.2808399; // feet
	    var interval = timestamp - lastTimestamp;
	    if (interval == 0.0 || interval == timestamp) {
		interval = 1.0;	// 1sec ensure no division by zero
		lastAltitude = altitude;  // first call, no change
	    }
	    climbRate = (altitude - lastAltitude) / interval;
            climbRateAngle = climbRate * 0.086;
	    // save for next calculation
	    lastTimestamp = timestamp;
	    lastAltitude = altitude;
        }
    }
}
