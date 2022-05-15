#include "Transponder.h"
#include <sys/time.h>

// fallbacks if unavailable from UI settings
#define UFO_GROUP_IPv4	"239.255.43.21"
#define UFO_GROUP_IPv6	"ff12::2115"
#define UFO_PORT	42424

Transponder::Transponder(MyMetrics *registry, MySettings *config, bool debug):
	QObject(nullptr), diagnostics(debug), settings(config), port(UFO_PORT),
	groupAddress4(QStringLiteral(UFO_GROUP_IPv4)),
	groupAddress6(QStringLiteral(UFO_GROUP_IPv6))
{
    if ((metrics = registry) != NULL) {
	metrics->add("transponder.send.count", "successfully sent messages");
	metrics->add("transponder.recv.count", "successfully recv'd messages");
	metrics->add("transponder.send.errors", "networking errors on sends");
	metrics->add("transponder.recv.errors", "networking errors on recvs");
	metrics->add("transponder.recv.corrupt", "bad datagrams received");
    }

    QUuid uuid;	/* Universally Unique ID for this transponder (one aircraft) */
    uuid.createUuid();
    memcpy(myself.senderUUID, uuid.toRfc4122().constData(), sizeof(myself.senderUUID));
    myself.length1 = myself.length2 = sizeof(IdentifiedFlyingObject);
    memcpy(myself.magic, "UFOP", sizeof(myself.magic));
}

void
Transponder::start(void)
{
    sendCount = (uint64_t *)metrics->map("transponder.send.count");
    sendErrors = (uint64_t *)metrics->map("transponder.send.errors");
    recvCount = (uint64_t *)metrics->map("transponder.recv.count");
    recvErrors = (uint64_t *)metrics->map("transponder.recv.errors");
    recvCorrupt = (uint64_t *)metrics->map("transponder.recv.corrupt");

    if (settings) {
	groupAddress4 = QHostAddress(settings->proximityIPv4());
	groupAddress6 = QHostAddress(settings->proximityIPv6());
	port = settings->proximityPort();
    }

    udpSocket4.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    udpSocket4.joinMulticastGroup(groupAddress4);

    udpSocket6.bind(QHostAddress::AnyIPv6, port, QUdpSocket::ShareAddress);
    udpSocket6.joinMulticastGroup(groupAddress6);

    connect(&udpSocket4, &QUdpSocket::readyRead,
            this, &Transponder::processPendingDatagrams);
    connect(&udpSocket6, &QUdpSocket::readyRead,
            this, &Transponder::processPendingDatagrams);

    connect(&timer, &QTimer::timeout, this, &Transponder::sendDatagram);

    timer.start(1000);
}

void
Transponder::stop(void)
{
    timer.stop();
    udpSocket4.close();
    udpSocket6.close();
}

void
Transponder::setTimeToLive(int ttl)
{
    // only set TTL on the IPv4 socket cos it changes the multicast scope
    udpSocket4.setSocketOption(QAbstractSocket::MulticastTtlOption, ttl);
}

void
Transponder::sendDatagram(void)
{
    encodeFlyingObject();

    if (udpSocket4.write(broadcast, broadcast.length()) == broadcast.length()) {
	if (metrics)
	    (*sendCount)++;
    } else {
	if (metrics)
	    (*sendErrors)++;
    }

    if (udpSocket6.state() != QAbstractSocket::BoundState)
	return;

    if (udpSocket6.write(broadcast, broadcast.length()) == broadcast.length()) {
	if (metrics)
	    (*sendCount)++;
    } else {
	if (metrics)
	    (*sendErrors)++;
    }
}

void
Transponder::processPendingDatagrams(void)
{
    QByteArray datagram;
    qint64 size;

    while (udpSocket4.hasPendingDatagrams()) {
	datagram.resize(int(udpSocket4.pendingDatagramSize()));
	size = udpSocket4.readDatagram(datagram.data(), datagram.size());
	    decodeFlyingObject(datagram, size);
    }

    while (udpSocket6.hasPendingDatagrams()) {
	datagram.resize(int(udpSocket6.pendingDatagramSize()));
	size = udpSocket6.readDatagram(datagram.data(), datagram.size());
	decodeFlyingObject(datagram, size);
    }
}

void
Transponder::decodeFlyingObject(QByteArray &message, size_t length)
{
    IdentifiedFlyingObject *tmp, *ufo = (IdentifiedFlyingObject *)message.constData();
    static IdentifiedFlyingObject *object;
    static size_t size;

    if (length > 0) {
	if (metrics)
	    recvCount++;
    } else {
	if (metrics)
	    recvErrors++;
    }

    if (length < sizeof(IdentifiedFlyingObject)) {
	fprintf(stderr, "Bad UFO packet length received: %zu < %zu\n",
		length, sizeof(IdentifiedFlyingObject));
	if (metrics)
	    recvCorrupt++;
	return;
    }
    if (memcmp(ufo->magic, "UFOP", 4) != 0) {
	fprintf(stderr, "Bad UFO packet magic received: %c%c%c%c\n",
		ufo->magic[0], ufo->magic[1], ufo->magic[2], ufo->magic[3]);
	if (metrics)
	    recvCorrupt++;
	return;
    }

    // Highwater-mark allocated object to reduce frequent allocations
    // of same or similar-sized buffers on receipt of these messages.
    if (length > size) {
	if ((tmp = (IdentifiedFlyingObject *)realloc(object, length)) == NULL)
	    return;
	object = tmp;
	size = length;
    }

    // Encode all numbers from little-endian form, copy out strings
    object->length1 = qFromLittleEndian(ufo->length1);
    object->timestamp = qFromLittleEndian(ufo->timestamp);
    object->identityLength = qFromLittleEndian(ufo->identityLength);
    object->latitudeDegrees = qFromLittleEndian(ufo->latitudeDegrees);
    object->latitudeMinutes = qFromLittleEndian(ufo->latitudeMinutes);
    object->longitudeDegrees = qFromLittleEndian(ufo->longitudeDegrees);
    object->longitudeMinutes = qFromLittleEndian(ufo->longitudeMinutes);
    object->groundSpeed = qFromLittleEndian(ufo->groundSpeed);
    object->altitude = qFromLittleEndian(ufo->altitude);
    object->length2 = qFromLittleEndian(ufo->length2);
    // Before copying any strings, double check all lengths received
    if (ufo->length1 != ufo->length2 ||
	ufo->length1 != length - sizeof(IdentifiedFlyingObject)) {
	fprintf(stderr, "Bad UFO packet length received: %u/%u vs %zu\n",
		ufo->length1, ufo->length2, length - sizeof(IdentifiedFlyingObject));
	if (metrics)
	    recvCorrupt++;
	return;
    }
    // Finally check null termination on the identity string as well
    if (ufo->identity[length - sizeof(IdentifiedFlyingObject)] != '\0') {
	fprintf(stderr, "Bad UFO packet identity terminal (%c)\n",
		ufo->identity[length - sizeof(IdentifiedFlyingObject)]);
	if (metrics)
	    recvCorrupt++;
	return;
    }

    memcpy(object->identity, ufo->identity, length - sizeof(IdentifiedFlyingObject));
    memcpy(object->senderUUID, ufo->senderUUID, sizeof(object->senderUUID));

    QUuid uuid;
    uuid.fromRfc4122(message + offsetof(IdentifiedFlyingObject, senderUUID));
    QByteArray bytes(message + offsetof(IdentifiedFlyingObject, identity));
    QString sender(bytes);

#ifdef DEBUG
	fprintf(stderr, "[%s] time=%lldms ID=%s\n"
			"latitude: %d %f longitude=%d %f\n"
			"heading=%u speed=%f altitude=%u\n",
			(const char *) uuid.toString().constData(),
			(long long) object->timestamp,
			(const char *) sender.constData(),
			object->latitudeDegrees, object->latitudeMinutes,
			object->longitudeDegrees, object->longitudeMinutes,
			object->heading, object->groundSpeed, object->altitude);
#endif

    emit updateFlyingObject(uuid, sender, object->timestamp, 
			    object->latitudeDegrees, object->latitudeMinutes,
			    object->heading,
			    object->longitudeDegrees, object->longitudeMinutes,
			    object->groundSpeed, object->altitude);
}

int
Transponder::setIdentity(QString &newIdentity)
{
    if (newIdentity.length() < 1)
	return -EINVAL;

    identity = newIdentity;

    // track the identity length in the protocol message structure
    myself.identityLength = identity.length() + 1;

    // track the total output length also (ID is the variable part)
    myself.length1 = sizeof(IdentifiedFlyingObject) + myself.identityLength;
    myself.length2 = myself.length1;
    return 0;
}

int
Transponder::setAltitude(unsigned int altitude)
{
    myself.altitude = altitude;
    return 0;
}

int
Transponder::setLongitude(bool south, unsigned int degrees, float minutes)
{
    if (degrees > 180)
	return -EINVAL;

    /* encode north using positive, south as negative numbers */
    myself.longitudeDegrees = degrees * (south ? -1 : 1);
    myself.longitudeMinutes = minutes;
    return 0;
}

int
Transponder::setLatitude(bool west, unsigned int degrees, float minutes)
{
    if (degrees > 90)
	return -EINVAL;

    /* encode east using positive, west as negative numbers */
    myself.latitudeDegrees = degrees * (west ? -1 : 1);
    myself.latitudeMinutes = minutes;
    return 0;
}

int
Transponder::setHeading(unsigned int degrees, float speed)
{
    if (degrees > 360 || speed < 0.0)
	return -EINVAL;
    myself.heading = degrees;
    myself.groundSpeed = speed;
    return 0;
}

// Prepare the broadcast buffer with details of this aircraft
// (i.e. location, speed, heading, identity information).
void
Transponder::encodeFlyingObject(void)
{
    // Sample the system clock and return milliseconds since the epoch
    // No timezone adjustment is performed - this will be used as UTC.
    struct timeval tv;
    gettimeofday(&tv, NULL);
    myself.timestamp = ((quint64)tv.tv_sec * 1000) + tv.tv_usec % 1000;

    // Produce final (endian-safe) buffer contents
    broadcast.resize(myself.length1);
    IdentifiedFlyingObject *out = (IdentifiedFlyingObject *)broadcast.data();

    // Strings first - these are endian neutral
    memcpy(out->magic, myself.magic, sizeof(out->magic));
    memcpy(out->identity, identity.constData(), identity.length());
    memcpy(out->senderUUID, myself.senderUUID, sizeof(out->senderUUID));

    // Encode all numbers in little-endian form
    qToLittleEndian(myself.length1, &out->length1);
    qToLittleEndian(myself.timestamp, &out->timestamp);
    qToLittleEndian(myself.identityLength, &out->identityLength);
    qToLittleEndian(myself.latitudeDegrees, &out->latitudeDegrees);
    qToLittleEndian(myself.latitudeMinutes, &out->latitudeMinutes);
    qToLittleEndian(myself.longitudeDegrees, &out->longitudeDegrees);
    qToLittleEndian(myself.longitudeMinutes, &out->longitudeMinutes);
    qToLittleEndian(myself.groundSpeed, &out->groundSpeed);
    qToLittleEndian(myself.altitude, &out->altitude);
    qToLittleEndian(myself.heading, &out->heading);
    qToLittleEndian(myself.length2, &out->length2);
}