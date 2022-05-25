#ifndef ALTIMU10_H
#define ALTIMU10_H

#include <QProcess>
#include "MyMetrics.h"
#include "MySettings.h"

class Altimu10 : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(float pitch READ getPitch NOTIFY pitchChanged)
    Q_PROPERTY(float roll READ getRoll NOTIFY rollChanged)
    Q_PROPERTY(float yaw READ getYaw NOTIFY yawChanged)
    Q_PROPERTY(float pressure READ getPressure NOTIFY pressureChanged)
    Q_PROPERTY(float temperature READ getTemperature NOTIFY temperatureChanged)

public:
    Altimu10(QString program,
	     MyMetrics *registry = NULL,
	     MySettings *config = NULL,
	     bool debug = false);
    ~Altimu10();

    void start();
    bool valid() { return success; }

    float getPitch() { return pitch; }
    float getRoll() { return roll; }
    float getYaw() { return yaw; }
    float getTemperature() { return temperature; }
    float getPressure() { return pressure; }

signals:
    void updatedHeading(float);
    void pitchChanged();
    void rollChanged();
    void yawChanged();
    void temperatureChanged();
    void pressureChanged();

protected:
    void tryRead();
    bool parse(const char *);
    void timerEvent(QTimerEvent*) override;

private:
    float temperature;
    float pressure;
    float pitch;
    float roll;
    float yaw;

    MySettings *settings;
    bool diagnostics;	// output debugging information
    bool success; // was previous parsing attempt successful
    QString command;

    // internal tests
    int temperatureDirection;
    int pressureDirection;
    int pitchDirection;
    int rollDirection;
    int yawDirection;
    int temperatureDelta;
    int pressureDelta;
    int pitchDelta;
    int rollDelta;
    int yawDelta;
    int shortTimer;
    int longTimer;

    // instrumentation
    MyMetrics *metrics;
    float *temperaturep;	// most recent sample
    float *pressurep;	// most recent sample
    uint64_t *errors;	// bad input counter
    uint64_t *count;	// success counter
};

#endif // ALTIMU10_H
