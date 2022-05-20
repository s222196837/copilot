#ifndef FLYINGOBJECT_H
#define FLYINGOBJECT_H

#include <QObject>
#include <QEasingCurve>
#include <QGeoCoordinate>

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
#define UFO_MSEC	1000	/* pulse rate */


class FlyingObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate position READ position WRITE setPosition NOTIFY positionChanged)

public:
    FlyingObject();
    void setPosition(const QGeoCoordinate &c);
    QGeoCoordinate position() const { return currentPosition; }

signals:
    void positionChanged();

private:
    QGeoCoordinate currentPosition;
    QEasingCurve easingCurve;
};

#endif // FLYINGOBJECT_H
