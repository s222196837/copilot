#include <QNmeaPositionInfoSource>
#include "GPS.h"

class FixSource : public QNmeaPositionInfoSource
{
public:
    FixSource() :
	QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode) {}

    bool parsePosInfoFromNmeaData(const char *data, int size,
		QGeoPositionInfo *posInfo, bool *hasFix) {
	return QNmeaPositionInfoSource::parsePosInfoFromNmeaData(data,
		size, posInfo, hasFix);
    }
};

GPS::GPS(QString program, MyMetrics *registry, bool debug):
	diagnostics(debug), success(false), command(program)
{
    fixSource = new FixSource();

    if ((metrics = registry) != NULL) {
	metrics->add("gps.altitude", "most recent altitude observation");
	metrics->add("gps.errors", "number of lines of bad GPS data");
	metrics->add("gps.count", "successfully parsed NMEA strings");
    }

    connect(this, &QProcess::readyReadStandardOutput, this, &GPS::tryRead);
}

GPS::~GPS()
{
    if (state() != QProcess::NotRunning) {
	kill();
	waitForFinished(20);
    }
    delete fixSource;
}

void
GPS::start()
{
    // prepare memory mapped metric pointers for live updating
    altitude = (uint64_t *)metrics->map("gps.altitude");
    errors = (uint64_t *)metrics->map("gps.errors");
    count = (uint64_t *)metrics->map("gps.count");

    // start the GPS data harvesting process
    QProcess::start(command, QStringList());
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

	if (fixSource->parsePosInfoFromNmeaData(bytes, line.length(),
			&info, &success) == false) {
	    if (metrics)
		(*errors)++;
	} else {
	    QGeoCoordinate coordinate = info.coordinate();
	    if (metrics) {
		(*altitude) = coordinate.altitude();
		(*count)++;
	    }
	    emit updatedCoordinate(coordinate);
	    emit updatedPosition(info.timestamp(), coordinate);
	}
    }
}
