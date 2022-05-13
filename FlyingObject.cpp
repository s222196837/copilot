#include "FlyingObject.h"

#define ANIMATION_DURATION 1000

FlyingObject::FlyingObject()
{
    easingCurve.setType(QEasingCurve::InOutQuad);
    easingCurve.setPeriod(ANIMATION_DURATION);
}

void FlyingObject::setPosition(const QGeoCoordinate &c)
{
    if (currentPosition == c)
        return;

    currentPosition = c;
    emit positionChanged();
}
