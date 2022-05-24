#ifndef RECEIVER_H
#define RECEIVER_H

#include <QtNetwork>
#include <QUuid>
#include "MyMetrics.h"
#include "MySettings.h"

class Receiver : public QObject
{
    Q_OBJECT

public:
    Receiver(MyMetrics *registry = NULL, MySettings *config = NULL, bool debug = false);
    void setFilter(QUuid uuid) { device = uuid; }
    void start();
    void stop();

signals:
    void updateFlyingObject(QUuid, QString, quint64,
			    double, double, double, double);

private slots:
    void processPendingDatagrams(void);

private:
    void decodeFlyingObject(QByteArray&, size_t);

    bool diagnostics;	// output debugging information

    MySettings *settings;
    int port;
    QHostAddress groupAddress4;
    QHostAddress groupAddress6;
    QUdpSocket udpSocket4;
    QUdpSocket udpSocket6;

    MyMetrics *metrics;
    uint64_t *count;	/* successfully received messages */
    uint64_t *errors;	/* networking errors on receive */
    uint64_t *corrupt;	/* corrupt datagrams received */

    QUuid device;	/* ignored entity (optional, self) */
};

#endif	/* RECEIVER_H */
