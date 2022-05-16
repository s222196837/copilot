#ifndef BUZZER_H
#define BUZZER_H

#include <QProcess>
#include "MyMetrics.h"

class Buzzer : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(int volume READ getVolume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool fastBeep READ getFastBeep WRITE setFastBeep NOTIFY soundEmitted)
    Q_PROPERTY(bool longBeep READ getLongBeep WRITE setLongBeep NOTIFY soundEmitted)

public:
    Buzzer(QString program, MyMetrics *registry = NULL, bool debug = false);
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

signals:
    void volumeChanged();
    void enabledChanged();
    void soundEmitted();

protected:
    void sendBeep(bool);
    void errorOccurred(QProcess::ProcessError);

private:
    int volume;	// percentage
    bool enabled;
    bool fastBeep;
    bool longBeep;

    bool diagnostics;	// output debugging information
    QString command;

    MyMetrics *metrics;
    uint64_t *errors;
    uint64_t *count;
};

#endif // BUZZER_H
