import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtLocation 5.15


ApplicationWindow {
    property var copilot: "COPILOT"
    property var dashColor: "#4b4b4b"

    id: window
    height: 700
    width: 500
    maximumHeight: height
    minimumHeight: height
    maximumWidth: width
    minimumWidth: width
    title: qsTr(" ")
    visible: true

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
	    id: mainHeading
	    text: copilot + "  -  " + Qt.formatTime(new Date(), "h:mm AP")
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
                text: qsTr("Pilot")
                width: parent.width
                onClicked: {
                    stackView.push("Pilot.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Aircraft")
                width: parent.width
                onClicked: {
                    stackView.push("Aircraft.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    stackView.push("Settings.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Proximity")
                width: parent.width
                onClicked: {
                    stackView.push("Proximity.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Flight Tracking")
                width: parent.width
                onClicked: {
                    stackView.push("Tracking.qml")
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

    Timer {
	interval: 10000	// every 10 seconds
	running: true
	repeat: true

	onTriggered: {
	    // h:mm:ss AP - for updating display containing seconds as well
	    mainHeading.text = copilot + "  -  " + Qt.formatTime(new Date(), "h:mm AP")
	}
    }
}
