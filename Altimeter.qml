import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var altitude: 0.0
    property var pressure: 28.0
    property var angleHand1: 0.0
    property var angleHand2: 0.0
    property var angleFace1: 0.0
    property var angleFace3: 0.0
  
    Button {
	background: Rectangle {
	    id: background
	    color: baseColor
	    anchors.fill: parent

	    Image {
		id: alt_case
		source: "qrc:/images/alt/alt_case.svg"
		height: length
		width: length
		z: 10
	    }

	    Image {
		id: alt_face_1
		source: "qrc:/images/alt/alt_face_1.svg"
		height: length
		width: length
		z: -50

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: -angleFace1
		}
	    }

	    Image {
		id: alt_face_2
		source: "qrc:/images/alt/alt_face_2.svg"
		height: length
		width: length
		z: -40
	    }

	    Image {
		id: alt_face_3
		source: "qrc:/images/alt/alt_face_3.svg"
		height: length
		width: length
		z: -30

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: angleFace3
		}
	    }

	    Image {
		id: alt_hand_1
		source: "qrc:/images/alt/alt_hand_1.svg"
		height: length
		width: length
		z: -20

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: angleHand1
		}
	    }

	    Image {
		id: alt_hand_2
		source: "qrc:/images/alt/alt_hand_2.svg"
		height: length
		width: length
		z: -10

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: angleHand2
		}
	    }
	}

        Component.onCompleted: {
            gps.positionChanged.connect(updateAltitude);
            altimu10.pressureChanged.connect(updatePressure);
        }

        function updatePressure() {
            // units: pascals to inches of mercury
            pressure = altimu10.pressure * 0.02953;
        }

        function updateAltitude() {
            // units: meters to feet
            altitude = gps.position.altitude / 3.28084;
            var alt = Math.ceil(altitude + 0.5);
            angleHand1 = altitude * 0.036;
            angleHand2 = (alt % 1000) * 0.36;
            angleFace1 = (pressure - 28.0) * 100.0;
            angleFace3 = altitude * 0.0036;
        }
    }
}
