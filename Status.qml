import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    property var charge: 0
    property var volume: 11
    property var gpsEnabled: 1
    property var wifiEnabled: 1

    property var testPattern: true
    property var end: "    "
    property var br: "  "

    function gps() {
        if (gpsEnabled == 1)
            return "\uF107" // globe
        return "\uF122"     // targeting
    }

    function wifi() {
        if (wifiEnabled == 1)
            return "\uF108" // signal
        return "\uF140"     // clock
    }

    function sound() {
        if (volume == 0)
            return "\uF136" // no sound
	if (volume <= 5)
            return "\uF137" // some sound
        return "\uF138"     // full sound
    }

    function battery() {
        if (charge == 0)
            return "\uF119" // battery empty
        if (charge == 1)
            return "\uF118" // battery one-quarter
        if (charge == 2)
            return "\uF117" // battery half
        if (charge == 3)
            return "\uF116" // battery three-quarters
        if (charge == 4)
            return "\uF115" // battery full
        return "\uF114"     // battery charging
    }

    id: statusBar
    font.bold: true
    font.family: "myfontello"
    text: gps() + br + wifi() + br + sound() + br + battery() + end

    // Testing - once every interval, change status of all elements

    Timer {
        property var chargeDirection : 1
        property var volumeDirection : 1
        property var count : 1

        interval: 1000

        repeat: testPattern
        running: testPattern

	onTriggered: {
	    if (++count % 10 == 0) {
		gpsEnabled = !gpsEnabled;
		wifiEnabled = !wifiEnabled;
	    }

	    // change battery usage charge
	    charge += (1 * chargeDirection);
	    if (charge <= 0 || charge >= 5) {
		chargeDirection *= -1;
	    }

	    // change sound volume
	    volume += (1 * volumeDirection);
	    if (volume <= 0 || volume >= 11) {
		volumeDirection *= -1;
	    }
	}
    }
}
