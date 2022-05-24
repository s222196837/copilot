import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    property var power: false
    property var charge: 100
    property var volume: 11
    property var gpsEnabled: true
    property var wifiEnabled: false
    property var soundEnabled: true
    property var end: "   "
    property var br: "  "

    function gps() {
        if (gpsEnabled)
            return br + "\uF107" // globe
        return "";
    }

    function wifi() {
        if (wifiEnabled)
            return br + "\uF108" // signal
        return "";
    }

    function sound() {
        if (!soundEnabled)
            return br + "\uF136" // no sound
	if (volume <= 5)
            return br + "\uF137" // some sound
        return br + "\uF138"     // full sound
    }

    function battery() {
        var icons = br
        if (power)
            icons = "\uF114" + br   // battery charging

        if (charge < 15)
            return icons + "\uF119" // battery empty
        if (charge < 30)
            return icons + "\uF118" // battery one-quarter
        if (charge < 55)
            return icons + "\uF117" // battery half
        if (charge < 80)
            return icons + "\uF116" // battery three-quarters

        return icons + "\uF115"     // battery full
    }

    id: statusBar
    font.bold: true
    font.family: "myfontello"
    text: gps() + wifi() + sound() + battery() + end

    // Testing - once every interval, change status of all elements

    Timer {
        property var chargeDirection : 1
        property var volumeDirection : 1
        property var count : 1

        interval: 1000

        repeat: settings.testsEnabled
        running: settings.testsEnabled

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
