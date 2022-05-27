#ifndef BUZZER_H
#define BUZZER_H

#include <QProcess>
#include "MyMetrics.h"
#include "MySettings.h"

class Buzzer : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(int volume READ getVolume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool fastBeep READ getFastBeep WRITE setFastBeep NOTIFY soundEmitted)
    Q_PROPERTY(bool longBeep READ getLongBeep WRITE setLongBeep NOTIFY soundEmitted)
    Q_PROPERTY(double intensity READ getIntensity WRITE setIntensity NOTIFY intensityChanged)

public:
    Buzzer(QString program,
	   MyMetrics *registry = NULL,
	   MySettings *config = NULL,
	   bool debug = false);
    ~Buzzer();

    void start();

    int getVolume() { return volume; }
    void setVolume(int percent);
    bool getEnabled() { return enabled; }
    void setEnabled(bool enable) { enabled = enable; }
    bool getFastBeep() { return fastBeep; }
    void setFastBeep(bool beep);
    bool getLongBeep() { return longBeep; }
    void setLongBeep(bool beep);
    double getIntensity() { return intensity; }
    void setIntensity(double rate);

public slots:
    void alarm();
    void vario(double);
    void updateAudibleAlarm();

signals:
    void soundEmitted();
    void volumeChanged();
    void enabledChanged();
    void intensityChanged();

protected:
    void sendBeep(bool);
    void errorOccurred(QProcess::ProcessError);

private:
    int volume;	// percentage
    bool enabled;
    bool fastBeep;
    bool longBeep;

    MySettings *settings;
    bool diagnostics;	// output debugging information
    bool audibleAlarm;
    double intensity;
    QString command;

    MyMetrics *metrics;
    uint64_t *alarms;
    uint64_t *varios;
    uint64_t *errors;
};

#endif // BUZZER_H
