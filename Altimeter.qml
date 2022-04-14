
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtGraphicalEffects 1.0

Item {
    property var baseColor: "#4b4b4b"
    property var dimensions: 164
    property var altitude: 0.0
    property var pressure: 0.0
  
    Button {
	style: ButtonStyle {
	    background: Rectangle {
		id: background
		color: baseColor
		anchors.fill: parent

		Image {
		    id: alt
		    visible: true
		    source: "qrc:/images/alt/alt.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: alt_case
		    visible: false
		    source: "qrc:/images/alt/alt_case.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: alt_face_1
		    visible: false
		    source: "qrc:/images/alt/alt_face_1.svg"
		    height: dimensions
		    width: dimensions
		}
		Image {
		    id: alt_face_2
		    visible: false
		    source: "qrc:/images/alt/alt_face_2.svg"
		    height: dimensions
		    width: dimensions
		}
		Image {
		    id: alt_face_3
		    visible: false
		    source: "qrc:/images/alt/alt_face_3.svg"
		    height: dimensions
		    width: dimensions
		}

		Image {
		    id: alt_hand_1
		    visible: false
		    source: "qrc:/images/alt/alt_hand_1.svg"
		    height: dimensions
		    width: dimensions
		}
		Image {
		    id: alt_hand_2
		    visible: false
		    source: "qrc:/images/alt/alt_hand_2.svg"
		    height: dimensions
		    width: dimensions
		}
	    }
	}
    }
}
