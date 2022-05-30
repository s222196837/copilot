#include <QNmeaPositionInfoSource>
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
	savedTimestamp(0.0), savedAltitude(0.0), longTimer(-1), shortTimer(-1)
{
    gpsSource = new GpsSource();

    if ((metrics = registry) != NULL) {
	metrics->addf("gps.climbrate", "most recent rate of climb");
	metrics->addf("gps.altitude", "most recent altitude sample");
	metrics->add("gps.errors", "number of lines of bad GPS data");
	metrics->add("gps.count", "successfully parsed NMEA strings");
    }

    if (settings && settings->testsEnabled()) {
	QGeoCoordinate coordinate(-38.342647, 144.319029, 0);	/* home */
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
	climbrate = metrics->mapf("gps.climbrate");
	altitudep = metrics->mapf("gps.altitude");
	errors = metrics->map("gps.errors");
	count = metrics->map("gps.count");
    } else {
	climbrate = &MyMetrics::unused.f;
	altitudep = &MyMetrics::unused.f;
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
	    (*count)++;
	    update();
	}
    }
}

void
GPS::update()
{
    QDateTime currentTimestamp = info.timestamp();
    QGeoCoordinate coordinate = info.coordinate();
    double currentAltitude = coordinate.altitude();
    double timestamp = currentTimestamp.toMSecsSinceEpoch() / 1000.0; // secs
    double altitude = currentAltitude * 3.2808399; // feet, aviation standard
    double interval = timestamp - savedTimestamp;
    double climbRate;

    if (interval == 0.0 || interval == timestamp) {
	interval = 1.0;  // 1 sec: preventing division by zero
	savedAltitude = altitude;  // first call, so no change
    }

    // calculate rate of climb in feet per second
    climbRate = (altitude - savedAltitude) / interval;

    // save for next calculation
    savedTimestamp = timestamp;
    savedAltitude = altitude;

    // update metrics with most recent values
    (*altitudep) = currentAltitude;
    (*climbrate) = climbRate;

    test.setCoordinate(coordinate);
    emit updatedPosition(currentTimestamp, coordinate);
    emit updatedClimbRate(climbRate);
    emit positionChanged();
}

//
// This timer is used in the dynamic testing mode only
//
void
GPS::timerEvent(QTimerEvent *event)
{
    // change directions on long timer
    if (event->timerId() == longTimer) {
	test.updateDirection();
    }
    // change location on short timer
    else {
	test.updatePosition();
	info.setCoordinate(test.coordinate());
	info.setTimestamp(QDateTime::currentDateTime());
	update();
    }
}

//
// Generate test data on the fly and inject it into the system
//

RandomMovement::RandomMovement():
	random(QRandomGenerator::global()),
	coord(-38.342647, 144.319029, 0), /* fixed start */
	latitudeDelta(0), longitudeDelta(0), altitudeDelta(0), headingDelta(0)
{
    updateDirection();
    updatePosition();
    heading = (random->generate() % 360);		// { 0 -> 359 }
}

void
RandomMovement::updateDirection()
{
    latitudeDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
    longitudeDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
    altitudeDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
    headingDelta = (random->generate() % 3) - 1;	// { -1, 0, 1 }
}

#define RADIUS		6378137.0
#define DELTA_DISTANCE	1.5	// incremental distance change

void
RandomMovement::updatePosition()
{
    double da, dn, de, dlat, dlon;

    dn = DELTA_DISTANCE * latitudeDelta;
    de = DELTA_DISTANCE * longitudeDelta;
    dlat = dn / RADIUS;
    dlon = de / (RADIUS * cos(M_PI * coord.latitude()/180.0));
    dlat *= 180.0 / M_PI;
    dlon *= 180.0 / M_PI;
    coord.setLatitude(coord.latitude() + dlat);
    coord.setLongitude(coord.longitude() + dlon);

    if (coord.altitude() > 3000)  // enforce a ceiling (meters)
	altitudeDelta = -1.0;
    da = (random->generate() % 25);  // change climb rate
    da = fabs(DELTA_DISTANCE * da * altitudeDelta);
    coord.setAltitude(coord.altitude() + da);

    heading += headingDelta;
    if (heading < 0 || heading >= 360)
	heading = random->generate() % 360;
}
