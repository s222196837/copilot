import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtLocation 5.15

ApplicationWindow {
    id: window
    height: 700
    width: 500
    visible: true
    title: qsTr(" ")

    property var dashColor: "#4b4b4b"

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }

	Battery {
	    id: batteryStatus
	    anchors.right: parent.right
	    anchors.verticalCenter: parent.verticalCenter
	}
    }

    footer: ToolBar {

	height: 170
	width: parent.width
	visible: true

        RowLayout {
	    id: instrumentsLayout
	    spacing: 0

	    Rectangle {
		color: dashColor
		Layout.fillWidth: true
		Layout.minimumWidth: 168
		Layout.minimumHeight: 168
		Layout.preferredWidth: 168
		Layout.preferredHeight: 168
                Attitude {
		    height: parent.height
		    width: parent.width
		    anchors.centerIn: parent
                    anchors.fill: parent
		}
	    }
	    Rectangle {
		color: dashColor
		Layout.fillWidth: true
		Layout.minimumWidth: 168
		Layout.minimumHeight: 168
		Layout.preferredWidth: 168
		Layout.preferredHeight: 168
                Altimeter {
		    height: parent.height
		    width: parent.width
		    anchors.centerIn: parent
                    anchors.fill: parent
		}
	    }
	    Rectangle {
		color: dashColor
		Layout.fillWidth: true
		Layout.minimumWidth: 168
		Layout.minimumHeight: 168
		Layout.preferredWidth: 168
		Layout.preferredHeight: 168
		VerticalSpeed {
		    height: parent.height
		    width: parent.width
		    anchors.centerIn: parent
                    anchors.fill: parent
		}
            }
        }
    }

    Drawer {
        id: drawer
	edge: Qt.LeftEdge
        width: window.width * 0.33
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Instruments")
                width: parent.width
                onClicked: {
                    stackView.push("instruments.ui.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    stackView.push("settings.ui.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "copilot.qml"
        anchors.fill: parent
    }
}
