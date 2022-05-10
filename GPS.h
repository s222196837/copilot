#ifndef GPS_H
#define GPS_H

#include <QProcess>
#include <QGeoPositionInfo>

class FixSource;

class GPS: public QProcess
{
    Q_OBJECT

public:
    GPS(QString s = QString("copilot-gps"));
    ~GPS();
    void start() { QProcess::start(command, QStringList()); }

    bool getFix() { return hasFix; }
    QGeoCoordinate getPosition() { return position.coordinate(); }

protected:
    void tryRead();

private:
    bool hasFix;
    QGeoPositionInfo position;

    QString command;
    FixSource *fixSource;
    unsigned long long errors;
    unsigned long long count;
};

#endif // GPS_H
