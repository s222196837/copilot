#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

class MySettings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(QString pilotName READ pilotName WRITE setPilotName NOTIFY pilotNameChanged)
    Q_PROPERTY(QString pilotNickname READ pilotNickname WRITE setPilotNickname NOTIFY pilotNicknameChanged)
    Q_PROPERTY(QString pilotRegistration READ pilotRegistration WRITE setPilotRegistration NOTIFY pilotRegistrationChanged)
    Q_PROPERTY(QString aircraftName READ aircraftName WRITE setAircraftName NOTIFY aircraftNameChanged)
    Q_PROPERTY(QString aircraftType READ aircraftType WRITE setAircraftType NOTIFY aircraftTypeChanged)
    Q_PROPERTY(bool soundEnabled READ soundEnabled WRITE setSoundEnabled NOTIFY soundEnabledChanged)
    Q_PROPERTY(int soundVolume READ soundVolume WRITE setSoundVolume NOTIFY soundVolumeChanged)
    Q_PROPERTY(bool wifiEnabled READ wifiEnabled WRITE setWifiEnabled NOTIFY wifiEnabledChanged)
    Q_PROPERTY(QString wifiNetwork READ wifiNetwork WRITE setWifiNetwork NOTIFY wifiNetworkChanged)
    Q_PROPERTY(QString wifiPassphrase READ wifiPassphrase WRITE setWifiPassphrase NOTIFY wifiPassphraseChanged)
    Q_PROPERTY(bool proximityEnabled READ proximityEnabled WRITE setProximityEnabled NOTIFY proximityEnabledChanged)
    Q_PROPERTY(QString proximityIPv4 READ proximityIPv4 WRITE setProximityIPv4 NOTIFY proximityIPv4Changed)
    Q_PROPERTY(QString proximityIPv6 READ proximityIPv6 WRITE setProximityIPv6 NOTIFY proximityIPv6Changed)
    Q_PROPERTY(int proximityPort READ proximityPort WRITE setProximityPort NOTIFY proximityPortChanged)
    Q_PROPERTY(bool proximityAudible READ proximityAudible WRITE setProximityAudible NOTIFY proximityAudibleChanged)
    Q_PROPERTY(int proximityDistance READ proximityDistance WRITE setProximityDistance NOTIFY proximityDistanceChanged)
    Q_PROPERTY(bool trackingEnabled READ trackingEnabled WRITE setTrackingEnabled NOTIFY trackingEnabledChanged)
    Q_PROPERTY(QString trackingURL READ trackingURL WRITE setTrackingURL NOTIFY trackingURLChanged)
    Q_PROPERTY(QString trackingKey READ trackingKey WRITE setTrackingKey NOTIFY trackingKeyChanged)

signals:
    void pilotNameChanged();
    void pilotNicknameChanged();
    void pilotRegistrationChanged();
    void aircraftNameChanged();
    void aircraftTypeChanged();
    void soundEnabledChanged();
    void soundVolumeChanged();
    void wifiEnabledChanged();
    void wifiNetworkChanged();
    void wifiPassphraseChanged();
    void proximityEnabledChanged();
    void proximityAudibleChanged();
    void proximityDistanceChanged();
    void proximityIPv4Changed();
    void proximityIPv6Changed();
    void proximityPortChanged();
    void trackingEnabledChanged();
    void trackingURLChanged();
    void trackingKeyChanged();

    void updatedProximityDistance(int);

public:
    MySettings(): QSettings() {}

    // pilot
    const QString pilotName() const;
    void setPilotName(const QString &);
    const QString pilotNickname() const;
    void setPilotNickname(const QString &);
    const QString pilotRegistration() const;
    void setPilotRegistration(const QString &);

    // aircraft
    const QString aircraftName() const;
    void setAircraftName(const QString &);
    const QString aircraftType() const;
    void setAircraftType(const QString &);

    // sound
    bool soundEnabled() const;
    void setSoundEnabled(bool);
    int soundVolume() const;
    void setSoundVolume(int);

    // wifi
    bool wifiEnabled() const;
    void setWifiEnabled(bool);
    const QString wifiNetwork() const;
    void setWifiNetwork(const QString &);
    const QString wifiPassphrase() const;
    void setWifiPassphrase(const QString &);

    // proximity
    bool proximityEnabled() const;
    void setProximityEnabled(bool);
    bool proximityAudible() const;
    void setProximityAudible(bool);
    int proximityDistance() const;
    void setProximityDistance(int);
    const QString proximityIPv4() const;
    void setProximityIPv4(const QString &);
    const QString proximityIPv6() const;
    void setProximityIPv6(const QString &);
    int proximityPort() const;
    void setProximityPort(int);

    // tracking
    bool trackingEnabled() const;
    void setTrackingEnabled(bool);
    const QString trackingURL() const;
    void setTrackingURL(const QString &);
    const QString trackingKey() const;
    void setTrackingKey(const QString &);
};

#endif // MYSETTINGS_H
