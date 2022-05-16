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
                    font.pixelSize: 20
                    checked: settings.soundEnabled
                    onToggled: {
                        var enabled = soundEnabledField.checked;
                        soundVolumeField.enabled = enabled;
                        settings.soundEnabled = enabled;
                    }
                }
                Dial {
                    id: soundVolumeField
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: settings.soundEnabled
                    value: settings.soundVolume
                    from: 1
                    to: 100
                    onMoved: settings.soundVolume = value
                }
                Switch {
                    id: wifiEnabledField
                    text: "Wi-Fi"
                    font.pixelSize: 20
                    checked: settings.wifiEnabled
                    onToggled: {
                        var enabled = wifiEnabledField.checked;
                        wifiNetworkLabel.enabled = enabled;
                        wifiNetworkField.enabled = enabled;
                        wifiPassphraseField.enabled = enabled;
                        wifiPassphraseLabel.enabled = enabled;
                        settings.wifiEnabled = enabled;
                    }
                }
                Label {
                    id: wifiNetworkLabel
                    text: "Network Name (SSID)"
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: settings.wifiEnabled
                }
                TextField {
                    id: wifiNetworkField
                    width: parent.width
                    text: settings.wifiNetwork
                    font.pixelSize: 20
                    enabled: settings.wifiEnabled
                    onAccepted: wifiPassphraseField.focus = true
                    onTextEdited: settings.wifiNetwork = text
                }
                Label {
                    id: wifiPassphraseLabel
                    text: "Passphrase (empty if none)"
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: settings.wifiEnabled
                }
                TextInput {
                    id: wifiPassphraseField
                    width: parent.width
                    text: settings.wifiPassphrase
                    font: wifiNetworkField.font
                    color: wifiNetworkField.color
                    cursorDelegate: wifiNetworkField.cursorDelegate
                    enabled: settings.wifiEnabled
                    echoMode: TextInput.Password
                    passwordMaskDelay: 1000
                    onAccepted: wifiNetworkField.focus = true
                    onTextEdited: settings.wifiPassphrase = text
                }
            }
        }
    }
}
