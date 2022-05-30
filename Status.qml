import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    property var gpsEnabled: false
    property var end: "   "
    property var br: "  "

    id: statusBar
    font.bold: true
    font.family: "myfontello"

    function mygps() {
        if (gpsEnabled)
            return br + "\uF107" // globe
        return "";
    }

    function mywifi() {
        if (settings.wifiEnabled)
            return br + "\uF108" // signal
        return "";
    }

    function mysound() {
        if (!settings.soundEnabled)
            return br + "\uF136" // no sound
        if (settings.soundVolume <= 50)
            return br + "\uF137" // some sound
        return br + "\uF138"     // full sound
    }

    function mybattery() {
        var icons = br
        if (battery.powered && battery.charge < 80)
            icons = "\uF114" + br   // battery charging

        if (battery.charge < 15)
            return icons + "\uF119" // battery empty
        if (battery.charge < 30)
            return icons + "\uF118" // battery one-quarter
        if (battery.charge < 55)
            return icons + "\uF117" // battery half
        if (battery.charge < 80)
            return icons + "\uF116" // battery three-quarters

        return icons + "\uF115"     // battery full
    }

    function refreshStatusBar() {
        statusBar.text = mygps() + mywifi() + mysound() + mybattery() + end
    }

    function gpsChanged() {
        gps.enabled = true
    }

    Component.onCompleted: {
        gps.positionChanged.connect(gpsChanged)
        battery.chargeChanged.connect(refreshStatusBar)
        settings.wifiEnabledChanged.connect(refreshStatusBar)
        settings.soundVolumeChanged.connect(refreshStatusBar)
        settings.soundEnabledChanged.connect(refreshStatusBar)
        refreshStatusBar()
    }

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
	    if (volume < 0 || volume > 100) {
		volumeDirection *= -1;
	    }
	}
    }
}
