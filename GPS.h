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

    bool valid() { return success; }
    QGeoCoordinate getPosition() { return position.coordinate(); }

protected:
    void tryRead();

private:
    QGeoPositionInfo position;

    bool success;
    QString command;
    FixSource *fixSource;
    unsigned long long errors;
    unsigned long long count;
};

#endif // GPS_H
