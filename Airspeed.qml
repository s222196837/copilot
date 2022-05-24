import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var airSpeed: 0.0
    property var airSpeedAngle: 0.0

    Button {
	background: Rectangle {
	    id: background
	    color: baseColor
	    anchors.fill: parent

	    Image {
		id: asi_case
		source: "qrc:/images/asi/asi_case.svg"
		height: length
		width: length
		z: 10
	    }

	    Image {
		id: asi_face
		source: "qrc:/images/asi/asi_face.svg"
		height: length
		width: length
		z: -20
	    }

	    Image {
		id: asi_hand
		source: "qrc:/images/asi/asi_hand.svg"
		height: length
		width: length
		z: -10

                transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: airSpeedAngle
                }
	    }
	}
    }

    // Testing - once every interval, change current airspeed

    Timer {
	property var airSpeedDirection : 1.0

	interval: 100
	repeat: settings.testsEnabled
	running: settings.testsEnabled

	onTriggered: {
	    // rotate (airspeed increase/descrease)
	    if (airSpeed <= 0.0 || airSpeed >= 235.0) {
		airSpeedDirection *= -1.0;
	    }
	    airSpeed += (10.0 * airSpeedDirection);

	    if (airSpeed < 40.0) {
		airSpeedAngle = 0.9 * airSpeed;
	    } else if (airSpeed < 70.0) {
		airSpeedAngle = 36.0 + 1.8 * (airSpeed - 40.0);
	    } else if (airSpeed < 130.0) {
		airSpeedAngle = 90.0 + 2.0 * (airSpeed - 70.0);
	    } else if (airSpeed < 160.0) {
		airSpeedAngle = 210.0 + 1.8 * (airSpeed - 130.0);
	    } else {
		airSpeedAngle = 264.0 + 1.2 * (airSpeed - 160.0);
	    }
	}
    }
}
