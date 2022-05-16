#ifndef BATTERY_H
#define BATTERY_H

#include <QProcess>
#include "MyMetrics.h"

class Battery : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(int charge READ getCharge NOTIFY chargeChanged)
    Q_PROPERTY(bool status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(bool powered READ getPowered NOTIFY poweredChanged)

public:
    Battery(QString program, MyMetrics *registry = NULL, bool debug = false);
    ~Battery();

    void start();
    bool valid() { return success; }

    int getCharge() { return charge; }
    bool getStatus() { return status; }
    bool getPowered() { return powered; }

signals:
    void statusChanged();
    void chargeChanged();
    void poweredChanged();

protected:
    void tryRead();
    bool parse(const char *);

private:
    int charge;
    bool status;
    bool powered;

    bool diagnostics;	// output debugging information
    bool success; // was previous parsing attempt successful
    QString command;

    MyMetrics *metrics;
    uint64_t *errors;
    uint64_t *count;
};

#endif // BATTERY_H
