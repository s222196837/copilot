#ifndef FLYINGOBJECTS_H
#define FLYINGOBJECTS_H

#include <QHash>
#include "GPS.h"
#include "Buzzer.h"
#include "Receiver.h"

// Unidentified Flying Object Protocol (UFOP)
typedef struct IdentifiedFlyingObject {		// Bytes|Details
    unsigned char 	magic[4];		//  04  |  "UFOP"
    quint32		length1;		//  04  |  bytes
    quint64		timestamp;		//  08  |  milliseconds UTC
    unsigned char 	senderUUID[32];		//  32  |  UUID
    quint16		identityLength;		//  02  |  bytes
    quint16		reservedPadding;	//  02  |  zeroed bytes
    double		latitude;		//  08  |  degrees (+=N,-=S)
    double		longitude;		//  08  |  degrees (+=E,-=W)
    double		altitude;		//  08  |  meters AMSL
    double		heading;		//  08  |  degrees
    quint32		length2;		//  04  |  bytes (dup)
    unsigned char	identity[0];		//  XX  |  UTF8 CSV
} IdentifiedFlyingObject;

// default values for UFO UDP socket setup
#define UFO_GROUP_IPv4	"239.255.43.21"
#define UFO_GROUP_IPv6	"ff12::2115"
#define UFO_PORT	42424

// default values for UFO related functionality
#define UFO_PULSE	1000	/* location frequency in milliseconds */
#define UFO_MAXID	65535	/* maximum identifier length in bytes */
#define UFO_ALERT	10.0	/* proximity alert distance in meters */


class FlyingObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate NOTIFY positionChanged)
    Q_PROPERTY(double bearing READ bearing WRITE setBearing NOTIFY bearingChanged)

signals:
    void nameChanged();
    void positionChanged();
    void bearingChanged();

public:
    FlyingObject(QUuid);
    void update(QString, quint64, double, double, double, double, bool);
    double altitude() { return myAltitude; }

    const QString name() const { return myName; }
    void setName(const QString &n) { myName = n; }
    const QGeoCoordinate coordinate() const;
    void setCoordinate(const QGeoCoordinate &);
    double bearing() { return myHeading; }
    void setBearing(double h) { myHeading = h; }

private:
    QString		myName;
    quint64		myTimestamp;
    double		myLatitude;
    double		myLongitude;
    double		myAltitude;
    double		myHeading;
    QUuid		myUuid;
};


class FlyingObjects : public QObject
{
    Q_OBJECT

public:
    FlyingObjects(GPS *, Buzzer *, Receiver *, bool);

signals:
    void objectAppeared(FlyingObject *);

private slots:
    void updateFlyingObject(QUuid, QString, quint64,
			    double, double, double, double);
    void proximityCheck();

private:
    GPS *gps;
    Buzzer *buzzer;
    QHash<QUuid, FlyingObject*> others;
    bool diagnostics;	// verbose mode
};

#endif // FLYINGOBJECTS_H
