#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QtCore>
#include <QtNetwork>
#include "FlyingObject.h"
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

    void setSources(GPS *, Altimu10 *);
    const QByteArray device() const { return hardware; }

public slots:
    void updatePosition();
    void updateHeading();
    void updateIdentity();

private slots:
    void setTimeToLive(int newTtl);
    void sendDatagram(void);

private:
    void encodeFlyingObject(void);

    bool diagnostics;	// output debugging information

    MySettings *settings;
    int port;
    QHostAddress groupAddress4;
    QHostAddress groupAddress6;
    QUdpSocket udpSocket4;
    QUdpSocket udpSocket6;
    QTimer timer;

    MyMetrics *metrics;
    uint64_t *count;	/* successfully sent messages */
    uint64_t *errors;	/* networking errors on sends */

    QString identity;
    IdentifiedFlyingObject myself; /* location/heading/identity detail */
    QByteArray broadcast;	/* preallocated buffer for broadcasting */
    QByteArray hardware;	/* UUID string representing this device */

    GPS *positionSource;
    Altimu10 *headingSource;
};

#endif	/* TRANSMITTER_H */
