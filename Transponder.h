#ifndef TRANSPONDER_H
#define TRANSPONDER_H

#include <QtCore>
#include <QtNetwork>
#include "MyMetrics.h"
#include "MySettings.h"

// Unidentified Flying Object Protocol (UFOP)
typedef struct IdentifiedFlyingObject {		// Bytes|Details
    unsigned char 	magic[4];		//  04  |  "UFOP"
    quint32		length1;		//  04  |  bytes
    quint64		timestamp;		//  08  |  milliseconds UTC
    unsigned char 	senderUUID[32];		//  32  |  UUID
    quint16		identityLength;		//  02  |  bytes
    qint16		latitudeDegrees;	//  02  |  degrees (+=N,-=S)
    float		latitudeMinutes;	//  04  |  minutes
    quint16		heading;		//  02  |  degrees
    qint16		longitudeDegrees;	//  02  |  degrees (+=E,-=W)
    float		longitudeMinutes;	//  04  |  minutes
    float		groundSpeed;		//  04  |  meters/second
    quint32		altitude;		//  04  |  meters AMSL
    quint32		length2;		//  04  |  bytes (dup)
    unsigned char	identity[0];		//  XX  |  UTF8 CSV
} IdentifiedFlyingObject;

class Transponder : public QObject
{
    Q_OBJECT

public:
    Transponder(MyMetrics *registry = NULL, MySettings *config = NULL, bool debug = false);
    void start();
    void stop();

public slots:
    int setIdentity(QString &identity);
    int setAltitude(unsigned int altitude);
    int setLongitude(bool south, unsigned int degrees, float minutes);
    int setLatitude(bool west, unsigned int degrees, float minutes);
    int setHeading(unsigned int degrees, float speed);

signals:
    void updateFlyingObject(QUuid, QString, quint64, qint16, float,
			    quint16, qint16, float, float, quint32);

private slots:
    void setTimeToLive(int newTtl);
    void sendDatagram(void);
    void processPendingDatagrams(void);

private:
    void decodeFlyingObject(QByteArray&, size_t);
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
    uint64_t *sendCount;	/* successfully sent messages */
    uint64_t *recvCount;	/* successfully recv messages */
    uint64_t *sendErrors;	/* networking errors on sends */
    uint64_t *recvErrors;	/* networking errors on recvs */
    uint64_t *recvCorrupt;	/* corrupt datagrams received */

    QString identity;
    IdentifiedFlyingObject myself; /* location/heading/identity detail */
    QByteArray broadcast;	/* preallocated buffer for broadcasting */
};

#endif	/* TRANSPONDER_H */
