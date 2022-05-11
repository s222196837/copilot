#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QTime>
#include <QBasicTimer>
#include <QDebug>
#include <QProcess>
#include <QFontDatabase>
#include <QEasingCurve>
#include <QGeoCoordinate>

#include "Altimu10.h"
#include "GPS.h"

#define ANIMATION_DURATION 4000

//! [PlaneController1]
class PlaneController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate position READ position WRITE setPosition NOTIFY positionChanged)
//! [PlaneController1]
    //! [C++Pilot1]
    Q_PROPERTY(QGeoCoordinate from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QGeoCoordinate to READ to WRITE setTo NOTIFY toChanged)
    //! [C++Pilot1]

public:
    PlaneController()
    {
        easingCurve.setType(QEasingCurve::InOutQuad);
        easingCurve.setPeriod(ANIMATION_DURATION);

	fprintf(stderr, "PlaneController()\n");
    }

#if 0
    void positionUpdated(const QGeoPositionInfo &update)
    {
	QGeoCoordinate coordinates = update.coordinate();
	fprintf(stderr, "positionUpdated: altitude=%.3f lat=%.3f long=%.3f\n",
			coordinates.altitude(),
			coordinates.latitude(),
			coordinates.longitude());
    }
#endif

    void setFrom(const QGeoCoordinate& from)
    {
        fromCoordinate = from;
    }

    QGeoCoordinate from() const
    {
        return fromCoordinate;
    }

    void setTo(const QGeoCoordinate& to)
    {
        toCoordinate = to;
    }

    QGeoCoordinate to() const
    {
        return toCoordinate;
    }

    void setPosition(const QGeoCoordinate &c) {
        if (currentPosition == c)
            return;

        currentPosition = c;
        emit positionChanged();
    }

    QGeoCoordinate position() const
    {
        return currentPosition;
    }

    Q_INVOKABLE bool isFlying() const {
        return timer.isActive();
    }

//! [C++Pilot2]
public slots:
    void startFlight()
    {
        if (timer.isActive())
            return;

        startTime = QTime::currentTime();
        finishTime = startTime.addMSecs(ANIMATION_DURATION);

        timer.start(15, this);
        emit departed();
    }
//! [C++Pilot2]

    void swapDestinations() {
        if (currentPosition == toCoordinate) {
            // swap destinations
            toCoordinate = fromCoordinate;
            fromCoordinate = currentPosition;
        }
    }

signals:
    void positionChanged();
    void arrived();
    void departed();
    void toChanged();
    void fromChanged();

protected:
    void timerEvent(QTimerEvent *event) override
    {
        if (!event)
            return;

        if (event->timerId() == timer.timerId())
            updatePosition();
        else
            QObject::timerEvent(event);
    }

private:
    //! [C++Pilot3]
    void updatePosition()
    {
#if 0
        // simple progress animation
        qreal progress;
        QTime current = QTime::currentTime();
        if (current >= finishTime) {
            progress = 1.0;
            timer.stop();
        } else {
            progress = ((qreal)startTime.msecsTo(current) / ANIMATION_DURATION);
        }

        setPosition(QWebMercator::coordinateInterpolation(
                          fromCoordinate, toCoordinate, easingCurve.valueForProgress(progress)));

        if (!timer.isActive())
            emit arrived();
#endif
    }
    //! [C++Pilot3]

private:
    QGeoCoordinate currentPosition;
    QGeoCoordinate fromCoordinate, toCoordinate;	// TODO: nuke
    QBasicTimer timer;
    QTime startTime, finishTime;
    QEasingCurve easingCurve;
//! [PlaneController2]
    // ...
};
//! [PlaneController2]

//! [PlaneControllerMain]
int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QFontDatabase fontDatabase;
    if (fontDatabase.addApplicationFont(":/fonts/myfontello.ttf") == -1)
	qWarning() << "Failed to load myfontello.ttf";

    GPS gps;
    Altimu10 altimu10;

    PlaneController glider;
    PlaneController oslo2berlin;
    PlaneController berlin2london;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gps", &gps);
    engine.rootContext()->setContextProperty("altimu10", &altimu10);
    engine.rootContext()->setContextProperty("glider", &glider);
    engine.rootContext()->setContextProperty("oslo2Berlin", &oslo2berlin);
    engine.rootContext()->setContextProperty("berlin2London", &berlin2london);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    gps.start();
    altimu10.start();

    return app.exec();
}
//! [PlaneControllerMain]

#include "main.moc"
