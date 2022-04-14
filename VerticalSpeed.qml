
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtGraphicalEffects 1.0

Item {
    property var baseColor: "#4b4b4b"
    property var dimensions: 164
    property var climbrate: 0.0
  
    Button {
	style: ButtonStyle {
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
}
