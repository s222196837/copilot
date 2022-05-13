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
            height: ufoFields.height + 24

            onClicked: focus = true

            Column {
                id: ufoFields
                spacing: 15
                x: 12
                y: 12
                width: parent.width - 26

                Switch {
                    id: proximityEnabledField
                    text: "Proximity Monitoring"
                    font.pixelSize: 20
                    checked: settings.proximityEnabled
                    onToggled: {
                        var enabled = proximityEnabledField.checked;
                        ufoIPv4GroupLabel.enabled = enabled;
                        ufoIPv4GroupField.enabled = enabled;
                        ufoIPv6GroupLabel.enabled = enabled;
                        ufoIPv6GroupField.enabled = enabled;
                        ufoPortLabel.enabled = enabled;
                        ufoPortField.enabled = enabled;
                        settings.proximityEnabled = enabled;
                    }
                }
                Label {
                    id: ufoPortLabel
                    text: "UDP Multicast Port Number"
                    font.pixelSize: 22
                    enabled: settings.proximityEnabled
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
                    id: ufoPortField
                    width: parent.width
                    text: settings.proximityPort
                    font.pixelSize: 20
                    inputMethodHints: Qt.ImhDigitsOnly
                    enabled: settings.proximityEnabled
                    onAccepted: ufoIPv4GroupField.focus = true
                    onTextEdited: settings.proximityPort = text
                }
                Label {
                    id: ufoIPv4GroupLabel
                    text: "UDP IPv4 Group Address"
                    font.pixelSize: 22
                    enabled: settings.proximityEnabled
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
                    id: ufoIPv4GroupField
                    width: parent.width
                    text: settings.proximityIPv4
                    font.pixelSize: 20
                    enabled: settings.proximityEnabled
                    onAccepted: ufoIPv6GroupField.focus = true
                    onTextEdited: settings.proximityIPv4 = text
                }
                Label {
                    id: ufoIPv6GroupLabel
                    text: "UDP IPv6 Group Address"
                    font.pixelSize: 22
                    enabled: settings.proximityEnabled
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
                    id: ufoIPv6GroupField
                    width: parent.width
                    text: settings.proximityIPv6
                    font.pixelSize: 20
                    enabled: settings.proximityEnabled
                    onAccepted: ufoPortField.focus = true
                    onTextEdited: settings.proximityIPv6 = text
                }
            }
        }
    }
}
