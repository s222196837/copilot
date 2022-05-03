import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    property var charge: 0
    property var testPattern: true

    function getUnicode() {
        if (charge == 0)
            return "\uF244" // battery empty
        else if (charge == 1)
            return "\uF243" // battery one-quarter
        else if (charge == 2)
            return "\uF242" // battery half
        else if (charge == 3)
            return "\uF241" // battery three-quarters
        else if (charge == 4)
            return "\uF240" // battery full
        else
            return "\uF1E6" // battery charging
    }

    id: batteryLabel
    text: "     " + getUnicode() + "     "
    font.family: "fontello"
    font.bold: true

    // Testing - once every interval, change charging status

    Timer {
        property var chargeDirection : 1

        interval: 1000
        repeat: testPattern
        running: testPattern

	onTriggered: {
	    // update (change battery usage charge)
	    charge += (1 * chargeDirection);
	    if (charge <= 0 || charge >= 5) {
		chargeDirection *= -1;
	    }
	}
    }
}
