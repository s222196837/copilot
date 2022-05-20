#include "FlyingObject.h"
#include "Receiver.h"

Receiver::Receiver(MyMetrics *registry, MySettings *config, bool debug):
	QObject(nullptr), diagnostics(debug), settings(config), port(UFO_PORT),
	groupAddress4(QStringLiteral(UFO_GROUP_IPv4)),
	groupAddress6(QStringLiteral(UFO_GROUP_IPv6)),
	device()
{
    if ((metrics = registry) != NULL) {
	metrics->add("receiver.count", "successfully recieved messages");
	metrics->add("receiver.errors", "networking errors on receives");
	metrics->add("receiver.corrupt", "bad UDP datagrams received");
    }
}

void
Receiver::start(void)
{
    count = (uint64_t *)metrics->map("receiver.count");
    errors = (uint64_t *)metrics->map("receiver.errors");
    corrupt = (uint64_t *)metrics->map("receiver.corrupt");

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
            this, &Receiver::processPendingDatagrams);
    connect(&udpSocket6, &QUdpSocket::readyRead,
            this, &Receiver::processPendingDatagrams);

    //if (diagnostics)
    fprintf(stderr, "Ready to receive datagrams from %s ",
		    (const char *)groupAddress4.toString().toLatin1());
    if (udpSocket6.state() == QAbstractSocket::BoundState)
	fprintf(stderr, "and %s ",
		    (const char *)groupAddress6.toString().toLatin1());
    fprintf(stderr, "on port %d\n", port);
}

void
Receiver::stop(void)
{
    udpSocket4.close();
    udpSocket6.close();
}

void
Receiver::processPendingDatagrams(void)
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
Receiver::decodeFlyingObject(QByteArray &message, size_t length)
{
    fprintf(stderr, "UFO recv len=%zu\n", length);

    IdentifiedFlyingObject *tmp, *ufo = (IdentifiedFlyingObject *)message.constData();
    static IdentifiedFlyingObject *object;
    static size_t size;

    if (length > 0) {
	if (metrics)
	    (*count)++;
    } else {
	if (metrics)
	    (*errors)++;
    }

    if (length < sizeof(IdentifiedFlyingObject)) {
	fprintf(stderr, "Bad UFO packet length received: %zu < %zu\n",
		length, sizeof(IdentifiedFlyingObject));
	if (metrics)
	    (*corrupt)++;
	return;
    }
    if (memcmp(ufo->magic, "UFOP", 4) != 0) {
	fprintf(stderr, "Bad UFO packet magic received: %c%c%c%c\n",
		ufo->magic[0], ufo->magic[1], ufo->magic[2], ufo->magic[3]);
	if (metrics)
	    (*corrupt)++;
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
    object->reservedPadding = 0;
    object->latitude = qFromLittleEndian(ufo->latitude);
    object->longitude = qFromLittleEndian(ufo->longitude);
    object->altitude = qFromLittleEndian(ufo->altitude);
    object->heading = qFromLittleEndian(ufo->heading);
    object->length2 = qFromLittleEndian(ufo->length2);
    // Before copying any strings, double check all lengths received
    if (ufo->length1 != ufo->length2 || ufo->length1 != length ||
	length < sizeof(IdentifiedFlyingObject)) {
	fprintf(stderr, "Bad UFO packet length received: %u/%u vs %zu\n",
		ufo->length1, ufo->length2, length - sizeof(IdentifiedFlyingObject));
	if (metrics)
	    (*corrupt)++;
	return;
    }
    // Finally check null termination on the identity string as well
    if (ufo->identity[length - sizeof(IdentifiedFlyingObject)] != '\0') {
	fprintf(stderr, "Bad UFO packet identity terminal (%c)\n",
		ufo->identity[length - sizeof(IdentifiedFlyingObject)]);
	if (metrics)
	    (*corrupt)++;
	return;
    }

    memcpy(object->identity, ufo->identity, length - sizeof(IdentifiedFlyingObject));
    memcpy(object->senderUUID, ufo->senderUUID, sizeof(object->senderUUID));

    QUuid uuid;
    uuid.fromRfc4122(message + offsetof(IdentifiedFlyingObject, senderUUID));
    QByteArray bytes(message + offsetof(IdentifiedFlyingObject, identity));
    QString sender(bytes);

    // if (diagnostics)
	fprintf(stderr, "[%s] time=%lldms ID=%s heading=%f\n"
			"latitude=%f longitude=%f altitude=%f\n",
			(const char *) uuid.toString().constData(),
			(long long) object->timestamp,
			(const char *) sender.constData(), object->heading,
			object->latitude, object->longitude, object->altitude);

    emit updateFlyingObject(uuid, sender, object->timestamp, object->heading,
			object->latitude, object->longitude, object->altitude);
}
