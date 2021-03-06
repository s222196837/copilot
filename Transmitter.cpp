#include <QStringBuilder>
#include "Transmitter.h"
#include <sys/time.h>

Transmitter::Transmitter(MyMetrics *registry, MySettings *config, bool debug):
	QObject(nullptr), diagnostics(debug), longTimer(-1), shortTimer(-1),
	settings(config), port(UFO_PORT),
	groupAddress4(QStringLiteral(UFO_GROUP_IPv4)),
	groupAddress6(QStringLiteral(UFO_GROUP_IPv6))
{
    udpSocket4.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    udpSocket6.bind(QHostAddress(QHostAddress::AnyIPv6), udpSocket4.localPort());

    if ((metrics = registry) != NULL) {
	metrics->add("transmitter.count", "successfully sent messages");
	metrics->add("transmitter.errors", "networking errors on sends");
    }

    /* Universally Unique ID for this transmitter (one aircraft) */
    hardware = hardware.createUuid();
    QByteArray device = hardware.toRfc4122();

    memcpy(myself.senderUUID, device.constData(), sizeof(myself.senderUUID));
    myself.length1 = myself.length2 = sizeof(IdentifiedFlyingObject);
    memset(&myself.timestamp, 0, sizeof(myself.timestamp));
    memcpy(myself.magic, "UFOP", sizeof(myself.magic));

    if (config) {
	updateIdentity();	// initialize (before signals)
	connect(config, &MySettings::pilotNameChanged,
			this, &Transmitter::updateIdentity);
	connect(config, &MySettings::pilotNicknameChanged,
			this, &Transmitter::updateIdentity);
	connect(config, &MySettings::pilotRegistrationChanged,
			this, &Transmitter::updateIdentity);
    }
}

Transmitter::~Transmitter()
{
    stop();
}

void
Transmitter::start(void)
{
    if (metrics) {
	count = metrics->map("transmitter.count");
	errors = metrics->map("transmitter.errors");
    } else {
	count = errors = &MyMetrics::unused.ull;
    }

    if (settings) {
	groupAddress4 = QHostAddress(settings->proximityIPv4());
	groupAddress6 = QHostAddress(settings->proximityIPv6());
	port = settings->proximityPort();
    }

    if (diagnostics) {
	fprintf(stderr, "Ready to multicast datagrams to %s ",
		    (const char *)groupAddress4.toString().toLatin1());
	if (udpSocket6.state() == QAbstractSocket::BoundState)
	    fprintf(stderr, "and %s ",
		    (const char *)groupAddress6.toString().toLatin1());
	fprintf(stderr, "on port %d\n", port);
    }

    if (settings && settings->testsEnabled()) {
	shortTimer = startTimer(505);   // 0.5 seconds
	longTimer = startTimer(2525);   // 2.5 seconds
    }

    connect(&timer, &QTimer::timeout, this, &Transmitter::sendDatagram);

    setTimeToLive((UFO_PULSE / 1000) * 10);
    timer.setInterval(UFO_PULSE);
    timer.start();
}

void
Transmitter::stop(void)
{
    if (settings && settings->testsEnabled()) {
	killTimer(shortTimer);
	killTimer(longTimer);
    }
    timer.stop();
    udpSocket4.close();
    udpSocket6.close();
}

void
Transmitter::setTimeToLive(int ttl)
{
    // only set TTL on the IPv4 socket as it changes the multicast scope
    udpSocket4.setSocketOption(QAbstractSocket::MulticastTtlOption, ttl);
}

void
Transmitter::sendDatagram(void)
{
    if (myself.timestamp == 0) {
	if (diagnostics)
	    fprintf(stderr, "Skip sending UDP pulse, no timestamp yet\n");
	return;
    }

    encodeFlyingObject();

    IdentifiedFlyingObject *out = (IdentifiedFlyingObject *)broadcast.data();

    if (diagnostics)
	fprintf(stderr, "UDP pulse %llu [len=%zu] identity=%s [len=%u/%zu]\n",
		    (unsigned long long)*count, (size_t)broadcast.length(),
		    out->identity, myself.identityLength,
		    sizeof(IdentifiedFlyingObject));

    if (udpSocket4.writeDatagram(broadcast, groupAddress4, port) == broadcast.length())
	(*count)++;
    else
	(*errors)++;

    if (udpSocket6.state() != QAbstractSocket::BoundState)
	return;

    if (udpSocket6.writeDatagram(broadcast, groupAddress6, port) == broadcast.length())
	(*count)++;
    else
	(*errors)++;
}

void
Transmitter::updatedPosition(QDateTime timestamp, QGeoCoordinate position)
{
    myself.altitude = position.altitude();
    myself.latitude = position.latitude();
    myself.longitude = position.longitude();
    myself.timestamp = timestamp.toMSecsSinceEpoch();
}

void
Transmitter::updatedHeading(float heading)
{
    myself.heading = heading;
}

void
Transmitter::updateIdentity()
{
    QString name, nick, rego;
    QChar separator = QChar('|');

    name = settings->pilotName();
    name.remove(separator);
    nick = settings->pilotNickname();
    nick.remove(separator);
    rego = settings->pilotRegistration();
    rego.remove(separator);

    updatedIdentity(name, nick, rego);
}

void
Transmitter::updatedIdentity(QString name, QString nick, QString rego)
{
    QChar separator = QChar('|');
    identity = name % separator % nick % separator % rego % separator;
    
    // keep the final identity string locally in the object
    if (identity.length() >= UFO_MAXID)
	identity.resize(UFO_MAXID - 1);

    // track the identity length in the protocol message structure
    myself.identityLength = identity.length() + 1;

    // track the total output length also (ID is the variable part)
    myself.length1 = sizeof(IdentifiedFlyingObject) + myself.identityLength;
    myself.length2 = myself.length1;
}

// Prepare the broadcast buffer with details of this aircraft
// (i.e. location, speed, heading, identity information).
void
Transmitter::encodeFlyingObject(void)
{
    // Produce final (endian-safe) buffer contents
    broadcast.resize(myself.length1);
    IdentifiedFlyingObject *out = (IdentifiedFlyingObject *)broadcast.data();

    // Strings first - these are endian neutral
    memcpy(out->magic, myself.magic, sizeof(out->magic));
    memcpy(out->identity, identity.toUtf8().constData(), identity.length());
    out->identity[identity.length()] = '\0';
    memcpy(out->senderUUID, myself.senderUUID, sizeof(out->senderUUID));
    out->reservedPadding = 0;

    // Encode all numbers in little-endian form
    qToLittleEndian(myself.length1, &out->length1);
    qToLittleEndian(myself.timestamp, &out->timestamp);
    qToLittleEndian(myself.identityLength, &out->identityLength);
    qToLittleEndian(myself.latitude, &out->latitude);
    qToLittleEndian(myself.longitude, &out->longitude);
    qToLittleEndian(myself.altitude, &out->altitude);
    qToLittleEndian(myself.heading, &out->heading);
    qToLittleEndian(myself.length2, &out->length2);
}

//
// This timer is used in the dynamic testing mode only
//
void
Transmitter::timerEvent(QTimerEvent *event)
{
    // change directions on long timer
    if (event->timerId() == longTimer) {
        test.updateDirection();
    }
    // change location on short timer
    else {
        test.updatePosition();
	updatedPosition(QDateTime::currentDateTime(), test.coordinate());
	updatedHeading(test.getHeading());
    }
}
