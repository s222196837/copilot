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

GPS::GPS(QString program) : hasFix(false), errors(0), count(0)
{
    connect(this, &QProcess::readyReadStandardOutput, this, &GPS::tryRead);
    fixSource = new FixSource();
    command = program;
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
GPS::tryRead()
{
    while (canReadLine()) {
	QByteArray line = readLine();
	const char *bytes = (const char *)line.constData();

	if (strncmp(bytes, "$GNGGA", 6) != 0)
	    continue;

	fprintf(stderr, "GPS: %s", bytes);
	if (fixSource->parsePosInfoFromNmeaData(bytes, line.length(),
			&position, &hasFix) == true)
	    count++;
	else
	    errors++;
    }
}
