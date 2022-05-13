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

                Switch {
                    id: trackingEnabledField
                    text: "Live Tracking"
                    font.pixelSize: 20
                    checked: settings.soundEnabled
                    onToggled: {
                        var enabled = trackingEnabledField.checked;
                        trackingUrlLabel.enabled = enabled;
                        trackingUrlField.enabled = enabled;
                        trackingKeyLabel.enabled = enabled;
                        trackingKeyField.enabled = enabled;
                        settings.trackingEnabled = enabled;
                    }
                }
                Label {
		    id: trackingUrlLabel
                    text: "URL"
                    font.pixelSize: 22
                    enabled: settings.trackingEnabled
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
		    id: trackingUrlField
                    width: parent.width
                    text: settings.trackingURL
                    font.pixelSize: 20
                    enabled: settings.trackingEnabled
                    onAccepted: trackingKeyField.focus = true
                    onTextEdited: settings.trackingURL = text
                }
                Label {
                    id: trackingKeyLabel
                    text: "Authentication Key"
                    font.pixelSize: 22
                    enabled: settings.trackingEnabled
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextInput {
                    id: trackingKeyField
                    width: parent.width
                    text: settings.trackingKey
                    font: trackingUrlField.font
                    color: trackingUrlField.color
                    cursorDelegate: trackingUrlField.cursorDelegate
                    enabled: settings.trackingEnabled
                    echoMode: TextInput.Password
                    passwordMaskDelay: 1000
                    onAccepted: trackingUrlField.focus = true
                    onTextEdited: settings.trackingKey = text
                }
            }
        }
    }
}
