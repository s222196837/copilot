#ifndef GPS_H
#define GPS_H

#include <QProcess>
#include <QGeoPositionInfo>
#include <QRandomGenerator>
#include "MyMetrics.h"
#include "MySettings.h"

class GpsSource;

class RandomMovement
{
public:
    RandomMovement();

    void updateDirection();
    void updatePosition();

    const QGeoCoordinate coordinate() const { return coord; }
    void setCoordinate(const QGeoCoordinate c) { coord = c; }
    float getHeading() { return heading; }

private:
    QRandomGenerator *random;
    QGeoCoordinate coord;
    int latitudeDelta;
    int longitudeDelta;
    int altitudeDelta;
    int headingDelta;
    float heading;
};

class GPS: public QProcess
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timestamp READ getTimestamp NOTIFY positionChanged);
    Q_PROPERTY(QGeoCoordinate position READ getPosition NOTIFY positionChanged);

public:
    GPS(QString command,
	MyMetrics *registry = NULL,
	MySettings *config = NULL,
	bool debug = false);
    ~GPS();

    void start();
    bool valid() { return success; }
    const QDateTime getTimestamp() { return info.timestamp(); }
    const QGeoCoordinate getPosition() { return info.coordinate(); }

signals:
    void updatedPosition(QDateTime, QGeoCoordinate);
    void updatedClimbRate(double);
    void positionChanged();

protected:
    void tryRead();
    void update();
    void timerEvent(QTimerEvent*) override;

private:
    QGeoPositionInfo info;
    GpsSource *gpsSource;

    MySettings *settings;
    bool diagnostics;	// output debugging information
    bool success; // was previous parsing attempt successful
    QString command;

    // climb rate calculation
    double savedTimestamp;
    double savedAltitude;

    // internal testing
    int longTimer;
    int shortTimer;
    RandomMovement test;

    // instrumentation
    MyMetrics *metrics;
    float *climbrate;	// most recently calculated
    float *altitudep;	// most recently sampled
    uint64_t *errors;	// bad input counter
    uint64_t *count;	// success counter
};

#endif // GPS_H
