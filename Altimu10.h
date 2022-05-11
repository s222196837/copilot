#ifndef ALTIMU10_H
#define ALTIMU10_H

#include <QProcess>

class Altimu10 : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(float pitch READ getPitch NOTIFY pitchChanged)
    Q_PROPERTY(float roll READ getRoll NOTIFY rollChanged)
    Q_PROPERTY(float yaw READ getYaw NOTIFY yawChanged)
    Q_PROPERTY(float pressure READ getPressure NOTIFY pressureChanged)
    Q_PROPERTY(float temperature READ getTemperature NOTIFY temperatureChanged)

public:
    Altimu10(QString program = QString("copilot-altimu10"));
    ~Altimu10();
    void start() { QProcess::start(command, QStringList()); }

    bool valid() { return success; }

    float getPitch() { return pitch; }
    float getRoll() { return roll; }
    float getYaw() { return yaw; }
    float getTemperature() { return temperature; }
    float getPressure() { return pressure; }

signals:
    void pitchChanged();
    void rollChanged();
    void yawChanged();
    void temperatureChanged();
    void pressureChanged();

protected:
    void tryRead();
    bool parse(const char *);

private:
    float pitch;
    float roll;
    float yaw;
    float temperature;
    float pressure;

    bool success; // was previous parsing attempt successful
    QString command;
    unsigned long long count;	// TODO: metrics
};

#endif // ALTIMU10_H
