#ifndef GPS_H
#define GPS_H

#include <QProcess>
#include <QGeoPositionInfo>
#include "MyMetrics.h"

class FixSource;

class GPS: public QProcess
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timestamp READ getTimestamp NOTIFY positionChanged);
    Q_PROPERTY(QGeoCoordinate position READ getPosition NOTIFY positionChanged);

public:
    GPS(QString command, MyMetrics *registry = NULL, bool debug = false);
    ~GPS();
    void start();

    bool valid() { return success; }
    const QDateTime getTimestamp() { return info.timestamp(); }
    const QGeoCoordinate getPosition() { return info.coordinate(); }

signals:
    void updatedPosition(QDateTime, QGeoCoordinate);
    void updatedCoordinate(QGeoCoordinate);
    void positionChanged();

protected:
    void tryRead();

private:
    QGeoPositionInfo info;
    FixSource *fixSource;

    bool diagnostics;	// output debugging information
    bool success; // was previous parsing attempt successful
    QString command;

    MyMetrics *metrics;
    float *altitude;	// most recent sample
    uint64_t *errors;	// bad input counter
    uint64_t *count;	// success counter
};

#endif // GPS_H
