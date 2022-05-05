import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.1

Rectangle {
    color: "#4b4b4b"

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: content.width
        contentHeight: content.height
        interactive: contentHeight > height
        flickableDirection: Flickable.VerticalFlick

        property real scrollMarginVertical: 20

        ScrollBar.vertical: ScrollBar {}

        MouseArea  {
            id: content
            width: flickable.width
            height: trackingFields.height + 24

            onClicked: focus = true

            Column {
                id: trackingFields
                spacing: 15
                x: 12
                y: 12
                width: parent.width - 26

                Label {
                    text: "URL"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                ComboBox {
		    id: trackingUrl
                    width: parent.width
		    model: [
			"http://test.livetrack24.com",
			"http://api.livetrack24.com"
		    ]
                }
                Label {
                    text: "Authentication Key"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
                    id: authenticationKey
                    width: parent.width
                    inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhLowercaseOnly | Qt.ImhUppercaseOnly
                    onAccepted: trackingUrl.focus = true
                }
            }
        }
    }
}
