import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var climbRateAngle: 0.086

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
            gps.updatedClimbRate.connect(updateClimbRate);
        }

        function updateClimbRate(climbRate) {
            climbRateAngle = climbRate * 0.086;
        }
    }
}
