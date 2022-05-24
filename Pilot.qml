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

        MouseArea {
            id: content
            width: flickable.width
            height: textEditors.height + 24

            onClicked: focus = true

            Column {
                id: textEditors
                spacing: 15
                x: 12
                y: 12
                width: parent.width - 26

                Label {
                    text: "Full Name"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
		    id: pilotNameField
                    width: parent.width
                    text: settings.pilotName
                    font.pixelSize: 20
                    onAccepted: pilotNicknameField.focus = true
                    onTextEdited: settings.pilotName = text
                }
                Label {
                    text: "Nick Name"
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
		    id: pilotNicknameField
                    width: parent.width
                    text: settings.pilotNickname
                    font.pixelSize: 20
                    onAccepted: registrationField.focus = true
                    onTextEdited: settings.pilotNickname = text
                }
                Label {
                    text: "Registration"
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
		    id: pilotRegistrationField
                    width: parent.width
                    text: settings.pilotRegistration
                    font.pixelSize: 20
                    onAccepted: pilotNameField.focus = true
                    onTextEdited: settings.pilotRegistration = text
                }
            }
        }
    }
}
