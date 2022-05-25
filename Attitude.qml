import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property var baseColor: "#4b4b4b"
    property var length: 164
    property var middle: length / 2

    property var roll: 0.0
    property var pitch: 0.0
    property var faceDeltaX_new: 0.0
    property var faceDeltaX_old: 0.0
    property var faceDeltaY_new: 0.0
    property var faceDeltaY_old: 0.0
    property var pixelsPerDegree: 1.7

    Button {
	background: Rectangle {
	    id: background
	    color: baseColor
	    anchors.fill: parent

	    Image {
		id: ai_back
		source: "qrc:/images/ai/ai_back.svg"
		height: length
		width: length
		z: -30

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: roll
		}
	    }

	    Image {
		id: ai_face
		source: "qrc:/images/ai/ai_face.svg"
		height: length
		width: length
		z: -20

		transform: [
		    Rotation {
		        origin.x: middle
		        origin.y: middle
		        angle: roll
		    },
		    Translate {
		        x: faceDeltaX_new - faceDeltaX_old
		        y: faceDeltaY_new - faceDeltaY_old
		    }
		]
	    }

	    Image {
		id: ai_ring
		source: "qrc:/images/ai/ai_ring.svg"
		height: length
		width: length
		z: -10

		transform: Rotation {
		    origin.x: middle
		    origin.y: middle
		    angle: roll
		}
	    }

	    Image {
		id: ai_case
		source: "qrc:/images/ai/ai_case.svg"
		height: length
		width: length
		z: 10
	    }
	}

        Component.onCompleted: {
            altimu10.pitchChanged.connect(updateAttitude);
        }

        function updateAttitude() {
            pitch = (altimu10.pitch - 75.0) / 8.0;
            roll = altimu10.roll - 180.0;
	    var rollRadians = Math.PI * roll / 180.0;
	    var delta = pixelsPerDegree * pitch;
	    faceDeltaX_new = delta * Math.sin(rollRadians);
	    faceDeltaY_new = delta * Math.cos(rollRadians);
        }
    }
}
