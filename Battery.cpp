#include <QTextStream>
#include "Battery.h"

Battery::Battery(QString program, MyMetrics *registry, bool debug):
	charge(0), status(false), powered(false),
	diagnostics(debug), success(false), command(program)
{
    if ((metrics = registry) != NULL) {
	metrics->add("battery.errors", "count of lines of bad battery data");
	metrics->add("battery.count", "successfully read battery sensors");
    }

    connect(this, &QProcess::readyReadStandardOutput, this, &Battery::tryRead);
}

Battery::~Battery()
{
    if (state() != QProcess::NotRunning) {
	kill(); // self-termination signal
	waitForFinished(20); // msec units
    }
}

void
Battery::start()
{
    // prepare memory mapped metric pointers for live updating
    errors = (uint64_t *)metrics->map("battery.errors");
    count = (uint64_t *)metrics->map("battery.count");

    QProcess::start(command, QStringList());
}

void
Battery::tryRead()
{
    while (canReadLine()) {
	QByteArray bytes = readLine();

	if (diagnostics)
	    fprintf(stderr, "BATTERY: %s", (const char *)bytes.constData());

	if (parse(bytes.constData()) == false) {
	    if (metrics)
		(*errors)++;
	} else {
	    if (metrics)
		(*count)++;
	}
    }
}

bool
Battery::parse(const char *line)
{
    char newstatus[16], newpwin[16], newpwin5v[16];
    bool newvalid, newpowered;
    int newcharge;

    int count = sscanf(line, "%15s %d %15s %15s\n",
		&newstatus[0], &newcharge, &newpwin[1], &newpwin5v[2]);

    if (count == 4) {
	newvalid = (strcmp(newstatus, "NO_ERROR") == 0);
	if (status != newvalid) {
	    status = newvalid;
	    emit statusChanged();
	}
	if (charge != newcharge) {
	    charge = newcharge;
	    emit chargeChanged();
	}
	newpowered = (strcmp(newpwin, "PRESENT") == 0 ||
		      strcmp(newpwin5v, "PRESENT") == 0);
	if (powered != newpowered) {
	    powered = newpowered;
	    emit poweredChanged();
	}

	success = true;
    } else {
	success = false;
    }
	
    return success;
}
