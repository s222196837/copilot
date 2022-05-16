#ifndef FLYINGOBJECT_H
#define FLYINGOBJECT_H

#include <QObject>
#include <QEasingCurve>
#include <QGeoCoordinate>

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
