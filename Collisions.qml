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

                Label {
                    text: "UDP IPv4 Group Address"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
                    id: ufoIPv4GroupField
                    width: parent.width
                    text: "239.255.43.21"
                    onAccepted: ufoIPv6GroupField.focus = true
                }
                Label {
                    text: "UDP IPv6 Group Address"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
                    id: ipv6GroupField
                    width: parent.width
                    text: "ff12::2115"
                    onAccepted: ufoPortField.focus = true
                }
                Label {
                    text: "UDP Port Number"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
                    id: ufoPortField
                    width: parent.width
                    text: "42424"
		    inputMethodHints: Qt.ImhDigitsOnly
                    onAccepted: ufoIPv4GroupField.focus = true
                }
            }
        }
    }
}
