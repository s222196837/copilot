#ifndef ALTIMU10_H
#define ALTIMU10_H

#include <QProcess>

class Altimu10 : public QProcess
{
    Q_OBJECT

public:
    Altimu10(QString program = QString("copilot-altimu10"));
    ~Altimu10();
    void start() { QProcess::start(command, QStringList()); }

    bool valid() { return success; }
    const float *getVelocity() { return velocity; }
    const float *getAcceleration() { return acceleration; }
    const float *getMagneticField() { return magnetic_field; }
    float getTemperature() { return temperature; }
    float getPressure() { return pressure; }

protected:
    void tryRead();
    bool parse(const char *);

private:
    float velocity[3];
    float acceleration[3];
    float magnetic_field[3];
    float temperature;
    float pressure;

    bool success; // was previous parsing attempt successful
    QString command;
    unsigned long long count;
};

#endif // ALTIMU10_H
