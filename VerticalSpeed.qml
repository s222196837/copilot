
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property var baseColor: "#4b4b4b"
    property var dimensions: 164
    property var climbrate: 0.0
  
    Button {
	    background: Rectangle {
		id: background
		color: baseColor
		anchors.fill: parent

		Image {
		    id: vsi
		    visible: true
		    source: "qrc:/images/vsi/vsi.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: vsi_case
		    visible: false
		    source: "qrc:/images/vsi/vsi_case.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: vsi_face
		    visible: false
		    source: "qrc:/images/vsi/vsi_face.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: vsi_hand
		    visible: false
		    source: "qrc:/images/vsi/vsi_hand.svg"
		    height: dimensions
		    width: dimensions
		}
	    }
    }
}
