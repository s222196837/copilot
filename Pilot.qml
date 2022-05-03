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
		    id: pilotFullNameField
                    width: parent.width
                    //enterKeyAction: EnterKeyAction.Next
                    onAccepted: nicknameField.focus = true
                }
                Label {
                    text: "Nick Name"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
		    id: pilotNickNameField
                    width: parent.width
                    //enterKeyAction: EnterKeyAction.Next
                    onAccepted: registrationField.focus = true
                }
                Label {
                    text: "Registration"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 22
                }
                TextField {
		    id: registrationField
                    width: parent.width
                    //enterKeyAction: EnterKeyAction.Next
                    onAccepted: pilotFullNameField.focus = true
                }
            }
        }
    }
}
