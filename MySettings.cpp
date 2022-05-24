#include "MySettings.h"

#define DEFAULT_PILOT_NAME		"Nathan Scott"
#define DEFAULT_PILOT_NICKNAME		"Nato"
#define DEFAULT_PILOT_REGISTRATION	"HG223455"
#define DEFAULT_AIRCRAFT_NAME		"STING-3"
#define DEFAULT_AIRCRAFT_TYPE		"Hang Glider"
#define DEFAULT_SOUND_ENABLED		true
#define DEFAULT_SOUND_VOLUME		6
#define DEFAULT_WIFI_ENABLED		false
#define DEFAULT_WIFI_NETWORK		"SkyFi"
#define DEFAULT_WIFI_PASSPHRASE		"SkyFiiFykS"
#define DEFAULT_PROXIMITY_ENABLED	false
#define DEFAULT_PROXIMITY_AUDIBLE	true
#define DEFAULT_PROXIMITY_DISTANCE	10
#define DEFAULT_PROXIMITY_IPV4		"239.255.43.21"
#define DEFAULT_PROXIMITY_IPV6		"ff12::2115"
#define DEFAULT_PROXIMITY_PORT		42424
#define DEFAULT_TRACKING_ENABLED	false
#define DEFAULT_TRACKING_URL		"http://test.livetrack24.com"
#define DEFAULT_TRACKING_KEY		"moc.24kcartevil.tset//:ptth"

void
MySettings::setDebugEnabled(bool enabled)
{
    diagnostics = enabled;
    emit debugEnabledChanged();
}

bool
MySettings::debugEnabled() const
{
    return diagnostics;
}

void
MySettings::setTestsEnabled(bool enabled)
{
    testpattern = enabled;
    emit testsEnabledChanged();
}

bool
MySettings::testsEnabled() const
{
    return testpattern;
}

void
MySettings::setPilotName(const QString &s)
{
    setValue("pilot/name", s);
    emit pilotNameChanged();
}

const QString
MySettings::pilotName() const
{
    return value("pilot/name", DEFAULT_PILOT_NAME).toString();
}

void
MySettings::setPilotNickname(const QString &s)
{
    setValue("pilot/nickname", s);
    emit pilotNicknameChanged();
}

const QString
MySettings::pilotNickname() const
{
    return value("pilot/nickname", DEFAULT_PILOT_NICKNAME).toString();
}

void
MySettings::setPilotRegistration(const QString &s)
{
    setValue("pilot/registration", s);
    emit pilotRegistrationChanged();
}

const QString
MySettings::pilotRegistration() const
{
    return value("pilot/registration", DEFAULT_PILOT_REGISTRATION).toString();
}

void
MySettings::setAircraftName(const QString &s)
{
    setValue("aircraft/name", s);
    emit aircraftNameChanged();
}

const QString
MySettings::aircraftName() const
{
    return value("aircraft/name", DEFAULT_AIRCRAFT_NAME).toString();
}

void
MySettings::setAircraftType(const QString &s)
{
    setValue("aircraft/type", s);
    emit aircraftTypeChanged();
}

const QString
MySettings::aircraftType() const
{
    return value("aircraft/type", DEFAULT_AIRCRAFT_TYPE).toString();
}

void
MySettings::setSoundEnabled(bool enabled)
{
    setValue("sound/enabled", enabled);
    emit soundEnabledChanged();
}

bool
MySettings::soundEnabled() const
{
    return value("sound/enabled", DEFAULT_SOUND_ENABLED).toBool();
}

void
MySettings::setSoundVolume(int volume)
{
    setValue("sound/volume", volume);
    emit soundVolumeChanged();
}

int
MySettings::soundVolume() const
{
    return value("sound/volume", DEFAULT_SOUND_VOLUME).toInt();
}

void
MySettings::setWifiEnabled(bool enabled)
{
    setValue("wifi/enabled", enabled);
    emit wifiEnabledChanged();
}

bool
MySettings::wifiEnabled() const
{
    return value("wifi/enabled", DEFAULT_WIFI_ENABLED).toBool();
}

void
MySettings::setWifiNetwork(const QString &s)
{
    setValue("wifi/network", s);
    emit wifiNetworkChanged();
}

const QString
MySettings::wifiNetwork() const
{
    return value("wifi/network", DEFAULT_WIFI_NETWORK).toString();
}

void
MySettings::setWifiPassphrase(const QString &s)
{
    setValue("wifi/passphrase", s);
    emit wifiPassphraseChanged();
}

const QString
MySettings::wifiPassphrase() const
{
    return value("wifi/passphrase", DEFAULT_WIFI_PASSPHRASE).toString();
}

void
MySettings::setProximityEnabled(bool enabled)
{
    setValue("proximity/enabled", enabled);
    emit proximityEnabledChanged();
}

bool
MySettings::proximityEnabled() const
{
    return value("proximity/enabled", DEFAULT_PROXIMITY_ENABLED).toBool();
}

void
MySettings::setProximityAudible(bool enabled)
{
    setValue("proximity/audible", enabled);
    emit proximityAudibleChanged();
}

bool
MySettings::proximityAudible() const
{
    return value("proximity/audible", DEFAULT_PROXIMITY_AUDIBLE).toBool();
}

void
MySettings::setProximityDistance(int meters)
{
    setValue("proximity/distance", meters);
    emit updatedProximityDistance(meters);
    emit proximityDistanceChanged();
}

int
MySettings::proximityDistance() const
{
    return value("proximity/distance", DEFAULT_PROXIMITY_DISTANCE).toInt();
}

void
MySettings::setProximityIPv4(const QString &s)
{
    setValue("proximity/ipv4", s);
    emit proximityIPv4Changed();
}

const QString
MySettings::proximityIPv4() const
{
    return value("proximity/ipv4", DEFAULT_PROXIMITY_IPV4).toString();
}

void
MySettings::setProximityIPv6(const QString &s)
{
    setValue("proximity/ipv6", s);
    emit proximityIPv6Changed();
}

const QString
MySettings::proximityIPv6() const
{
    return value("proximity/ipv6", DEFAULT_PROXIMITY_IPV6).toString();
}

void
MySettings::setProximityPort(int port)
{
    setValue("proximity/port", port);
    emit proximityPortChanged();
}

int
MySettings::proximityPort() const
{
    return value("proximity/port", DEFAULT_PROXIMITY_PORT).toInt();
}

void
MySettings::setTrackingEnabled(bool enabled)
{
    setValue("tracking/enabled", enabled);
    emit trackingEnabledChanged();
}

bool
MySettings::trackingEnabled() const
{
    return value("tracking/enabled", DEFAULT_TRACKING_ENABLED).toBool();
}

void
MySettings::setTrackingURL(const QString &url)
{
    setValue("tracking/url", url);
    emit trackingURLChanged();
}

const QString
MySettings::trackingURL() const
{
    return value("tracking/url", DEFAULT_TRACKING_URL).toString();
}

void
MySettings::setTrackingKey(const QString &key)
{
    setValue("tracking/key", key);
    emit trackingKeyChanged();
}

const QString
MySettings::trackingKey() const
{
    return value("tracking/key", DEFAULT_TRACKING_KEY).toString();
}
