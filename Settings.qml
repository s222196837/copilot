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
            height: settingsFields.height + 24

            onClicked: focus = true

            Column {
                id: settingsFields
                spacing: 15
                x: 12
                y: 12
                width: parent.width - 26

                Switch {
		    id: soundEnabledField
                    text: "Sound"
		    checked: mysettings.soundEnabled
                }
                Dial {
		    id: soundVolumeField
                    anchors.horizontalCenter: parent.horizontalCenter
		    value: 7
		    from: 0
		    to: 11
		}
                Switch {
		    id: wifiEnabledField
                    text: "Wi-Fi"
		    checked: false
                }
                Label {
                    text: "Network Name (SSID)"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
                    id: wifiNetwork
                    width: parent.width
                    onAccepted: wifiPassphrase.focus = true
                }
                Label {
                    text: "Passphrase (empty if none)"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
                    id: wifiPassphrase
                    width: parent.width
                    onAccepted: wifiNetwork.focus = true
                }
            }
        }
    }
}
