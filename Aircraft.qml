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
            height: aircraftFields.height + 24

            onClicked: focus = true

            Column {
                id: aircraftFields
                spacing: 15
                x: 12
                y: 12
                width: parent.width - 26

                Label {
                    text: "Aircraft"
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
                    id: aircraftNameField
                    width: parent.width
                    text: settings.aircraftName
                    font.pixelSize: 20
                    onAccepted: aircraftTypeField.focus = true
                    onTextEdited: settings.aircraftName = text
                }
                Label {
                    text: "Type"
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextField {
                    id: aircraftTypeField
                    width: parent.width
                    text: settings.aircraftType
                    font.pixelSize: 20
                    onAccepted: aircraftNameField.focus = true
                    onTextEdited: settings.aircraftType = text
                }
            }
        }
    }
}
