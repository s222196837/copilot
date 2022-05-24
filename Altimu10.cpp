#include <QTextStream>
#include "Altimu10.h"

Altimu10::Altimu10(QString program, MyMetrics *registry, bool debug):
	pitch(0.0), roll(0.0), yaw(0.0), temperature(0.0), pressure(0.0),
	diagnostics(debug), success(false), command(program)
{
    if ((metrics = registry) != NULL) {
	metrics->addf("altimu10.temperature", "most recent temperature sample");
	metrics->addf("altimu10.pressure", "most recent pressure sample");
	metrics->add("altimu10.errors", "count of lines of bad Altimu10 data");
	metrics->add("altimu10.count", "successfully read Altimu10 sensors");
    }

    connect(this, &QProcess::readyReadStandardOutput, this, &Altimu10::tryRead);
}

Altimu10::~Altimu10()
{
    if (state() != QProcess::NotRunning) {
	kill(); // self-termination signal
	waitForFinished(20); // msec units
    }
}

void
Altimu10::start()
{
    // prepare memory mapped metric pointers for live updating
    temperaturep = metrics->mapf("altimu10.temperature");
    pressurep = metrics->mapf("altimu10.pressure");
    errors = metrics->map("altimu10.errors");
    count = metrics->map("altimu10.count");

    QProcess::start(command, QStringList());
}

void
Altimu10::tryRead()
{
    while (canReadLine()) {
	QByteArray bytes = readLine();

	if (diagnostics)
	    fprintf(stderr, "ALTIMU10: %s", (const char *)bytes.constData());

	if (parse(bytes.constData()) == false) {
	    if (metrics)
		(*errors)++;
	} else {
	    if (metrics) {
		(*temperaturep) = temperature;
		(*pressurep) = pressure;
		(*count)++;
	    }
	}
    }
}

bool
Altimu10::parse(const char *line)
{
    float v[11];
    int count = sscanf(line, "%f %f %f %f %f %f %f %f %f %f %f\n",
		&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8],
		&v[9], &v[10]);

    if (count == 11) {
	//
	// Apply small dampening factors when sensors have minute changes;
	// casting the values from float to integer also assists somewhat.
	//
	if (abs((int)(pitch - v[0])) > 1) {
	    pitch = v[0];
	    emit pitchChanged();
	}
	if (abs((int)(roll - v[1])) > 1) {
	    roll = v[1];
	    emit rollChanged();
	}
	if (abs((int)(yaw - v[2])) > 1) {
	    yaw = v[2];
	    emit updatedHeading(yaw);
	    emit yawChanged();
	}
	if (abs((int)(temperature - v[9])) > 0) {
	    temperature = v[9];
	    emit temperatureChanged();
	}
	if (abs((int)(pressure - v[10])) > 0) {
	    pressure = v[10];
	    emit pressureChanged();
	}

	success = true;
    } else {
	success = false;
    }
	
    return success;
}
