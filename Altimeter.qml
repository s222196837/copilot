
import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var altitude: 0.0
    property var pressure: 28.0
    property var testPattern: false
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
		visible: true
		source: "qrc:/images/alt/alt_case.svg"
		height: length
		width: length
		z: 10
	    }

	    Image {
		id: alt_face_1
		visible: true
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
		visible: true
		source: "qrc:/images/alt/alt_face_2.svg"
		height: length
		width: length
		z: -40
	    }

	    Image {
		id: alt_face_3
		visible: true
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
		visible: true
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
		visible: true
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
    }

    // Testing - once every interval, increase altitude

    Timer {
	property var altitudeDirection : 1.0
	property var pressureDirection : 1.0

	interval: 100
	repeat: testPattern
	running: testPattern

	onTriggered: {
	    // rotate (change altitude and/or pressure)
	    altitude += (10.0 * altitudeDirection);
	    if (altitude <= 0.0 || altitude >= 3600.0) {
		altitudeDirection *= -1.0;
	    }
	    pressure += (0.01 * pressureDirection);
	    if (pressure < 28.0 || pressure > 31.5) {
		pressureDirection *= -1.0;
	    }

            var alt = Math.ceil(altitude + 0.5);
            angleHand1 = altitude * 0.036;
            angleHand2 = (alt % 1000) * 0.36;
            angleFace1 = (pressure - 28.0) * 100.0;
            angleFace3 = altitude * 0.0036;
        }
    }
}
