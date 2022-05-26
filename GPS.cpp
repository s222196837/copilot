#include <QNmeaPositionInfoSource>
#include <QRandomGenerator>
#include <QTimerEvent>
#include <math.h>
#include "GPS.h"

class GpsSource : public QNmeaPositionInfoSource
{
public:
    GpsSource() :
	QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode) {}

    bool parsePosInfoFromNmeaData(const char *data, int size,
		QGeoPositionInfo *posInfo, bool *hasFix) {
	return QNmeaPositionInfoSource::parsePosInfoFromNmeaData(data,
		size, posInfo, hasFix);
    }
};

GPS::GPS(QString program, MyMetrics *registry, MySettings *config, bool debug):
	settings(config), diagnostics(debug), success(false), command(program),
	latitudeDelta(0), longitudeDelta(0), altitudeDelta(0),
	shortTimer(-1), longTimer(-1)
{
    gpsSource = new GpsSource();

    if ((metrics = registry) != NULL) {
	metrics->addf("gps.altitude", "most recent altitude sample");
	metrics->add("gps.errors", "number of lines of bad GPS data");
	metrics->add("gps.count", "successfully parsed NMEA strings");
    }

    if (settings && settings->testsEnabled()) {
	QGeoCoordinate coordinate(-38.342647, 144.319029, 0);
	info.setCoordinate(coordinate);
    } else {
	connect(this, &QProcess::readyReadStandardOutput, this, &GPS::tryRead);
    }
}

GPS::~GPS()
{
    if (settings && settings->testsEnabled()) {
	killTimer(shortTimer);
	killTimer(longTimer);
    }
    else if (state() != QProcess::NotRunning) {
	kill(); // self-termination signal
	waitForFinished(20); // msec units
    }
    delete gpsSource;
}

void
GPS::start()
{
    // prepare memory mapped metric pointers for live updating
    if (metrics) {
	altitude = metrics->mapf("gps.altitude");
	errors = metrics->map("gps.errors");
	count = metrics->map("gps.count");
    } else {
	altitude = &MyMetrics::unused.f;
	errors = &MyMetrics::unused.ull;
	count = &MyMetrics::unused.ull;
    }

    if (settings && settings->testsEnabled()) {
	shortTimer = startTimer(500);	// 0.5 seconds
	longTimer = startTimer(2552);	// 2.5 seconds
    } else {
	// start the GPS data harvesting process
	QProcess::start(command, QStringList());
    }
}

void
GPS::tryRead()
{
    while (canReadLine()) {
	QByteArray line = readLine();
	const char *bytes = (const char *)line.constData();

	// only interested in position information, ignore all else
	if (strncmp(bytes, "$GNGGA", 6) != 0)
	    continue;

	if (diagnostics)
	    fprintf(stderr, "GPS: %s", bytes);

	if (gpsSource->parsePosInfoFromNmeaData(bytes, line.length(),
			&info, &success) == false) {
	    (*errors)++;
	} else {
	    (*altitude) = info.coordinate().altitude();
	    (*count)++;
	    update();
	}
    }
}

void
GPS::update()
{
    emit updatedPosition(info.timestamp(), info.coordinate());
    emit positionChanged();
}

//
// Generate test data on the fly and inject it into the system
//

#define RADIUS		6378137.0
#define DELTA_DISTANCE	1.5	// incremental distance change

void
GPS::timerEvent(QTimerEvent *event)
{
    QRandomGenerator *random = QRandomGenerator::global();

    // change directions on long timer
    if (event->timerId() == longTimer) {
	latitudeDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
	longitudeDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
	altitudeDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
    }
    // change location on short timer
    else {
	double da, dn, de, dlat, dlon;
	QGeoCoordinate c = info.coordinate();

	dn = DELTA_DISTANCE * latitudeDelta;
	de = DELTA_DISTANCE * longitudeDelta;
	dlat = dn / RADIUS;
	dlon = de / (RADIUS * cos(M_PI * c.latitude()/180.0));
	dlat *= 180.0 / M_PI;
	dlon *= 180.0 / M_PI;
	c.setLatitude(c.latitude() + dlat);
	c.setLongitude(c.longitude() + dlon);

	if (c.altitude() > 3000)	// enforce a ceiling
	    altitudeDelta = -1.0;
	da = (random->generate() % 25); // change climb rate
	da = fabs(DELTA_DISTANCE * da * altitudeDelta);
	c.setAltitude(c.altitude() + da);

	info.setCoordinate(c);
	info.setTimestamp(QDateTime::currentDateTime());

	update();
    }
}
