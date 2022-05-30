#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QtCore>
#include <QtNetwork>
#include <QDateTime>
#include <QGeoLocation>
#include "FlyingObjects.h"
#include "MySettings.h"
#include "MyMetrics.h"
#include "Altimu10.h"
#include "GPS.h"

class Transmitter : public QObject
{
    Q_OBJECT

public:
    Transmitter(MyMetrics *registry = NULL, MySettings *config = NULL, bool debug = false);
    ~Transmitter();
    void start();
    void stop();

    const QUuid device() const { return hardware; }

public slots:
    void updatedPosition(QDateTime, QGeoCoordinate);
    void updatedHeading(float);
    void updatedIdentity(QString, QString, QString);

private slots:
    void setTimeToLive(int newTtl);
    void sendDatagram(void);

protected:
    void timerEvent(QTimerEvent*) override;

private:
    void encodeFlyingObject(void);
    void updateIdentity();

    bool diagnostics;	// output debugging information

    // internal tests
    int longTimer;
    int shortTimer;
    RandomMovement test;

    // instrumentation
    MyMetrics *metrics;
    uint64_t *errors;	/* networking errors on sends */
    uint64_t *count;	/* successfully sent messages */

    MySettings *settings;
    int port;
    QHostAddress groupAddress4;
    QHostAddress groupAddress6;
    QUdpSocket udpSocket4;
    QUdpSocket udpSocket6;
    QTimer timer;
    QString identity;
    IdentifiedFlyingObject myself; /* location/heading/identity detail */
    QByteArray broadcast;	/* preallocated buffer for broadcasting */
    QUuid hardware;
};

#endif	/* TRANSMITTER_H */
