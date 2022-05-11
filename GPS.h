#ifndef GPS_H
#define GPS_H

#include <QProcess>
#include <QGeoPositionInfo>

class FixSource;

class GPS: public QProcess
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timestamp READ getTimestamp NOTIFY positionChanged);
    Q_PROPERTY(QGeoCoordinate position READ getPosition NOTIFY positionChanged);

public:
    GPS(QString s = QString("copilot-gps"));
    ~GPS();
    void start() { QProcess::start(command, QStringList()); }

    bool valid() { return success; }
    const QDateTime getTimestamp() { return info.timestamp(); }
    const QGeoCoordinate getPosition() { return info.coordinate(); }

signals:
    void positionChanged();

protected:
    void tryRead();

private:
    QGeoPositionInfo info;

    bool success;
    QString command;
    FixSource *fixSource;
    unsigned long long errors;	// TODO - PCP
    unsigned long long count;	// TODO - PCP
};

#endif // GPS_H
