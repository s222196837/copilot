#include <QTextStream>
#include "Altimu10.h"

Altimu10::Altimu10(QString program):
	pitch(0.0), roll(0.0), yaw(0.0), temperature(0.0), pressure(0.0),
	success(false), command(program), count(0)
{
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
Altimu10::tryRead()
{
    while (canReadLine()) {
	QByteArray bytes = readLine();

	fprintf(stderr, "ALTIMU10: %s", (const char *)bytes.constData());
	if (parse(bytes.constData()) == false)
	    continue;
	fprintf(stderr, "TEMPERATURE: %f", temperature);
	fprintf(stderr, " PRESSURE: %f\n", pressure);

	count++;
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
	    emit yawChanged();
	}
	if (abs((int)(pressure - v[9])) > 0) {
	    pressure = v[9];
	    emit pressureChanged();
	}
	if (abs((int)(temperature - v[10])) > 0) {
	    temperature = v[10];
	    emit temperatureChanged();
	}

	success = true;
    } else {
	success = false;
    }
	
    return success;
}
