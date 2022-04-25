
import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property var baseColor: "#4b4b4b"
    property var dimensions: 164
    property var airspeed: 0.0
  
    Button {
	    background: Rectangle {
		id: background
		color: baseColor
		anchors.fill: parent

		Image {
		    id: asi
		    visible: true
		    source: "qrc:/images/asi/asi.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: asi_case
		    visible: false
		    source: "qrc:/images/asi/asi_case.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: asi_face
		    visible: false
		    source: "qrc:/images/asi/asi_face.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: asi_hand
		    visible: false
		    source: "qrc:/images/asi/asi_hand.svg"
		    height: dimensions
		    width: dimensions
		}
	    }
    }
}
