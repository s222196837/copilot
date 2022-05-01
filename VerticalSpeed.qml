
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var climbRate: 0.0
    property var testPattern: false
    property var climbRateAngle : 0.0
  
    Button {
	background: Rectangle {
	    id: background
	    color: baseColor
	    anchors.fill: parent

	    Image {
		id: vsi_case
		visible: true
		source: "qrc:/images/vsi/vsi_case.svg"
		height: length
		width: length
		z: 10
	    }

	    Image {
		id: vsi_face
		visible: true
		source: "qrc:/images/vsi/vsi_face.svg"
		height: length
		width: length
		z: -20
	    }

	    Image {
		id: vsi_hand
		visible: true
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
    }

    // Testing - once every interval, change current rate of climb

    Timer {
        property var climbRateDirection : 1.0

        interval: 100
        repeat: testPattern
        running: testPattern

        onTriggered: {
            // rotate (climb faster/slower)
            if (climbRate <= -2000.0 || climbRate >= 2000.0) {
                climbRateDirection *= -1.0;
            }
            climbRate += (42.0 * climbRateDirection);
            climbRateAngle = climbRate * 0.086;
        }
    }
}
