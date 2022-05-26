#include "FlyingObjects.h"
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
    if (metrics) {
	count = metrics->map("receiver.count");
	errors = metrics->map("receiver.errors");
	corrupt = metrics->map("receiver.corrupt");
    } else {
	count = errors = corrupt = &MyMetrics::unused.ull;
    }

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

    if (diagnostics) {
	fprintf(stderr, "Ready to receive datagrams from %s ",
		    (const char *)groupAddress4.toString().toLatin1());
	if (udpSocket6.state() == QAbstractSocket::BoundState)
	    fprintf(stderr, "and %s ",
		    (const char *)groupAddress6.toString().toLatin1());
	fprintf(stderr, "on port %d\n", port);
    }
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
    if (diagnostics)
	fprintf(stderr, "UFO recv len=%zu\n", length);

    IdentifiedFlyingObject *tmp, *ufo = (IdentifiedFlyingObject *)message.constData();
    static IdentifiedFlyingObject *object;
    static size_t size;

    if (length > 0) {
	(*count)++;
    } else {
	(*errors)++;
    }

    if (length < sizeof(IdentifiedFlyingObject)) {
	if (diagnostics)
	    fprintf(stderr, "Bad UFO packet length received: %zu < %zu\n",
		    length, sizeof(IdentifiedFlyingObject));
	(*corrupt)++;
	return;
    }
    if (memcmp(ufo->magic, "UFOP", 4) != 0) {
	if (diagnostics)
	    fprintf(stderr, "Bad UFO packet magic received: %c%c%c%c\n",
		    ufo->magic[0], ufo->magic[1], ufo->magic[2], ufo->magic[3]);
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
	length <= sizeof(IdentifiedFlyingObject)) {
	if (diagnostics)
	    fprintf(stderr, "Bad UFO packet length received: %u/%u vs %zu\n",
		    ufo->length1, ufo->length2,
		    length - sizeof(IdentifiedFlyingObject));
	(*corrupt)++;
	return;
    }

    if (object->identityLength != length - sizeof(IdentifiedFlyingObject)) {
	if (diagnostics)
	    fprintf(stderr, "Bad UFO identity length received: %u vs %zu\n",
		    object->identityLength,
		    length - sizeof(IdentifiedFlyingObject));
	(*corrupt)++;
	return;
    }

    // Finally check null termination on the identity string as well
    if (ufo->identity[object->identityLength-1] != '\0') {
	if (diagnostics)
	    fprintf(stderr, "Bad UFO packet identity terminal (%c)\n",
		    ufo->identity[length - sizeof(IdentifiedFlyingObject)]);
	(*corrupt)++;
	return;
    }

    memcpy(object->identity, ufo->identity, object->identityLength);
    memcpy(object->senderUUID, ufo->senderUUID, sizeof(object->senderUUID));

    QUuid uuid = QUuid::fromRfc4122((const char *)object->senderUUID);
    QByteArray bytes((const char *)object->identity);
    QString string(bytes);

    QString pilot("UFO");
    QStringList sender = string.split(QChar('|'));

    // pick out the display identity
    if (sender.size() >= 2 && sender.at(1).length() > 0)	// nick
	pilot = sender.takeAt(1);
    else if (sender.size() >= 3 && sender.at(2).length() > 0)	// rego
	pilot = sender.takeAt(2);
    else if (sender.size() >= 1 && sender.at(0).length() > 0)	// name
	pilot = sender.takeAt(0);

    if (diagnostics)
	fprintf(stderr, "ID=%s[len=%d] time=%lldms heading=%f\n"
			"latitude=%f longitude=%f altitude=%f\n",
			(const char *)bytes.constData(), bytes.length(),
			(long long) object->timestamp, object->heading,
			object->latitude, object->longitude, object->altitude);

    // do not emit messages about our own local position updates
    if (device.isNull() == false && uuid != device)
	emit updateFlyingObject(uuid, pilot,
			object->timestamp, object->heading,
			object->latitude, object->longitude,
			object->altitude);
}
