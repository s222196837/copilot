import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.VirtualKeyboard 2.1

ApplicationWindow {
    property var copilot: "COPILOT"
    property var dashColor: "#4b4b4b"
    property var temperature: 0
    property var sep: " - "

    id: window
    height: 700
    width: 500
    maximumHeight: height
    minimumHeight: height
    maximumWidth: width
    minimumWidth: width
    title: qsTr(" ")
    visible: true

    function environment() {
	// report temperature in units of degrees celcius from Altimu10 sensor
	var temperatureString = temperature + "<sup>o</sup>C";
	// alternatively "h:mm:ss AP" displays updates containing seconds also
	return Qt.formatTime(new Date(), "h:mm AP") + ", " + temperatureString;
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    instruments.visible = true
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
	    id: mainHeading
	    textFormat: Text.RichText
	    text: copilot + sep + environment()
	    font.bold: true
            anchors.centerIn: parent
        }

	Status {
	    id: statusBar
	    anchors.right: parent.right
	    anchors.verticalCenter: parent.verticalCenter
	}

        Component.onCompleted: {
            altimu10.temperatureChanged.connect(temperatureChanged)
        }

        function temperatureChanged() {
	    temperature = Math.round(altimu10.temperature);
        }
    }

    footer: ToolBar {

	id: instruments
	height: 170
	width: parent.width
	visible: true

        RowLayout {
	    id: instrumentsLayout
	    spacing: 0

	    Rectangle {
		color: dashColor
		Layout.fillWidth: true
		Layout.minimumWidth: instruments.height - 2
		Layout.minimumHeight: instruments.height - 2
		Layout.preferredWidth: instruments.height - 2
		Layout.preferredHeight: instruments.height - 2
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
		Layout.minimumWidth: instruments.height - 2
		Layout.minimumHeight: instruments.height - 2
		Layout.preferredWidth: instruments.height - 2
		Layout.preferredHeight: instruments.height - 2
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
		Layout.minimumWidth: instruments.height - 2
		Layout.minimumHeight: instruments.height - 2
		Layout.preferredWidth: instruments.height - 2
		Layout.preferredHeight: instruments.height - 2
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
                    instruments.visible = false
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Aircraft")
                width: parent.width
                onClicked: {
                    stackView.push("Aircraft.qml")
                    instruments.visible = false
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    stackView.push("Settings.qml")
                    instruments.visible = false
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Proximity")
                width: parent.width
                onClicked: {
                    stackView.push("Proximity.qml")
                    instruments.visible = false
                    drawer.close()
                }
            }
        }
    }

    InputPanel {
        id: keyboard
	visible: active
	anchors.left: parent.left
	anchors.right: parent.right
	anchors.bottom: parent.bottom
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
	    mainHeading.text = copilot + sep + environment()
	}
    }
}
