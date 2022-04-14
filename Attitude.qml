
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtGraphicalEffects 1.0

Item {
    property var baseColor: "#4b4b4b"
    property var pitch: 0.0
    property var roll: 0.0
    property var size: 164
  
    Button {
	style: ButtonStyle {
	    background: Rectangle {
		id: background
		color: baseColor
		anchors.fill: parent

		Image {
		    id: ai
		    visible: true
		    source: "qrc:/images/ai/ai.svg"
		    height: size
		    width: size
		}

		Image {
		    id: ai_back
		    visible: false
		    source: "qrc:/images/ai/ai_back.svg"
		    height: size
		    width: size
		}

		Image {
		    id: ai_case
		    visible: false
		    source: "qrc:/images/ai/ai_case.svg"
		    height: size
		    width: size
		}

		Image {
		    id: ai_face
		    visible: false
		    source: "qrc:/images/ai/ai_face.svg"
		    height: size
		    width: size
		}

		Image {
		    id: ai_ring
		    visible: false
		    source: "qrc:/images/ai/ai_ring.svg"
		    height: size
		    width: size
		}
	    }
	}
    }
}
