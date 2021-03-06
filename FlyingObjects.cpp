#include "FlyingObjects.h"
#include <math.h>

// The Receiver has detected either a new flying object or an
// update to one we have seen before.  This does not include
// ourself - that position update comes from the GPS.  Check
// for proximity after all updates - someone could be moving
// into our too-close zone.
void
FlyingObjects::updateFlyingObject(QUuid uuid,
		QString name, quint64 timestamp, double heading,
		double latitude, double longitude, double altitude)
{
    FlyingObject *ufo = others.value(uuid, NULL);
    bool fresh = (ufo == NULL);

    if (fresh)
	ufo = new FlyingObject(uuid);

    ufo->update(name, timestamp, heading, latitude, longitude, altitude, fresh);

    if (fresh) {
	others.insert(uuid, ufo);
	emit objectAppeared(ufo);
    }

    // done last (after updating the map) so the pilot gets visuals
    proximityCheck();
}

void
FlyingObjects::proximityCheckByCoordinate(QDateTime d, QGeoCoordinate self)
{
    if (diagnostics)
	fprintf(stderr, "%s: time=%lld\n", __FUNCTION__, d.toSecsSinceEpoch());

    location = self;
    proximityCheck();
}

void
FlyingObjects::updatedProximityDistance(int meters)
{
    distance = meters;
    closing = meters * 10;
}

void
FlyingObjects::proximityCheck()
{
    double altitude = location.altitude();

    // iterate the hash and check how close every flying object is
    foreach (FlyingObject *object, others) {
	double incoming = fabs(altitude - object->altitude());

	// completely too high/low/faraway
	if (incoming > closing ||
	    location.distanceTo(object->coordinate()) > closing) {
	    object->setStatus("other");
	    continue;
	}

	// close - check whether warning or alarming
	if (incoming > distance ||
	    location.distanceTo(object->coordinate()) > distance) {
	    // closing in - warning
	    object->setStatus("close");
	} else {
	    // alarmingly close now
	    object->setStatus("alarm");
	    emit alarm();
	}
    }
}

FlyingObject::FlyingObject(QUuid uuid) : QObject(),
	myName(""), myStatus("other"), myTimestamp(0),
	myLatitude(0), myLongitude(0), myAltitude(0), myHeading(0), myUuid(uuid)
{
}

const QGeoCoordinate
FlyingObject::coordinate() const
{
    return QGeoCoordinate(myLatitude, myLongitude, myAltitude);
}

void
FlyingObject::setCoordinate(const QGeoCoordinate &coordinate)
{
    myLatitude = coordinate.latitude();
    myLongitude = coordinate.longitude();
    myAltitude = coordinate.altitude();
}

void
FlyingObject::setName(const QString &name)
{
    if (myName != name) {
	myName = name;
	emit nameChanged();
    }
}

void
FlyingObject::setStatus(const QString &status)
{
    if (myStatus != status) {
	myStatus = status;
	emit statusChanged();
    }
}

void
FlyingObject::update(QString name, quint64 timestamp, double heading,
	double latitude, double longitude, double altitude, bool first)
{
    myTimestamp = timestamp;
    setName(name);

    bool updatedPosition = false;
    if (myLatitude != latitude) {
	myLatitude = latitude;
	updatedPosition = true;
    }
    if (myLongitude != longitude) {
	myLongitude = longitude;
	updatedPosition = true;
    }
    if (myAltitude != altitude) {
	myAltitude = altitude;
	updatedPosition = true;
    }

    if (updatedPosition && !first)
	emit positionChanged();

    if (myHeading != heading) {
	myHeading = heading;
	if (!first)
	    emit bearingChanged();
    }
}
