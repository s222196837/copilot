#include <QTextStream>
#include <QRandomGenerator>
#include <QTimerEvent>
#include "Altimu10.h"

Altimu10::Altimu10(QString program,
	MyMetrics *registry, MySettings *config, bool debug):
	temperature(0.0), pressure(0.0), pitch(0.0), roll(0.0), yaw(0.0),
	settings(config), diagnostics(debug), command(program),
	temperatureDirection(1), pressureDirection(1),
	pitchDirection(1), rollDirection(1), yawDirection(1),
	temperatureDelta(0), pressureDelta(0),
	pitchDelta(0), rollDelta(0), yawDelta(0),
	shortTimer(-1), longTimer(-1)
{
    if ((metrics = registry) != NULL) {
	metrics->addf("altimu10.temperature", "most recent temperature sample");
	metrics->addf("altimu10.pressure", "most recent pressure sample");
	metrics->add("altimu10.errors", "count of lines of bad Altimu10 data");
	metrics->add("altimu10.count", "successfully read Altimu10 sensors");
    }

    if (settings && settings->testsEnabled())
	return;
    connect(this, &QProcess::readyReadStandardOutput, this, &Altimu10::tryRead);
}

Altimu10::~Altimu10()
{
    if (settings && settings->testsEnabled()) {
	killTimer(shortTimer);
	killTimer(longTimer);
    }
    else if (state() != QProcess::NotRunning) {
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

    if (settings && settings->testsEnabled()) {
	shortTimer = startTimer(515);   // 0.5 seconds
	longTimer = startTimer(5500);   // 5.5 seconds
	temperature = 21;
	pressure = 1016;
	pitch = 92;
	roll = 180;
	yaw = 180;
    } else {
	// start the Altimu10 harvesting process
	QProcess::start(command, QStringList());
    }
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
	return true;
    }
    return false;
}

void
Altimu10::timerEvent(QTimerEvent *event)
{
    QRandomGenerator *random = QRandomGenerator::global();

    // change directions and slow moving values on long timer
    if (event->timerId() == longTimer) {
	temperatureDelta = (random->generate() % 2);
	pressureDelta = (random->generate() % 5);
	pitchDelta = (random->generate() % 2);
	rollDelta = (random->generate() % 15);
	yawDelta = (random->generate() % 3);

	temperature += (temperatureDelta * temperatureDirection);
	if (temperature < -60.0 || temperature > 50.0)
	    temperatureDirection += -1.0;
	emit temperatureChanged();

	pressure += (pressureDelta * pressureDirection);
	if (pressure < 950.0 || pressure > 1050.0)
	    pressureDirection += -1.0;
	emit pressureChanged();
    }
    // change values on short timer
    else {
	pitch += (pitchDelta * pitchDirection);
	if (pitch <= -10.0 || pitch >= 10.0)
	    pitchDirection *= -1.0;

	emit pitchChanged();

	roll += (rollDelta * rollDirection);
	if (roll <= 0.0 || roll >= 360.0)
	    rollDirection *= -1.0;

	emit rollChanged();

	yaw += (yawDelta * yawDirection);
	if (yaw <= 0.0 || yaw >= 360.0)
	    yawDirection *= -1.0;

	emit updatedHeading(yaw);
	emit yawChanged();
    }

    if (diagnostics)
	fprintf(stderr, "%s: %.3f %.3f %.3f 0 0 0 0 0 0 %.3f %.3f\n",
		"ALTIMU10", pitch, roll, yaw, temperature, pressure);

}
