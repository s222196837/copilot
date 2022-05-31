#include <QtTest/QtTest>
#include <QSignalSpy>
#include "Transmitter.h"

class TestAircraft: public Transmitter
{
    Q_OBJECT
public:
    TestAircraft(QString command): Transmitter(), program(command), process() {}
    void start() { process.start(program, QStringList()); Transmitter::start(); }
    void stop() { if (!process.atEnd()) process.kill(); }

protected:
    void tryRead();

private:
    QString program;
    QProcess process;
};

void TestAircraft::tryRead()
{
    // each line of input contains: "name nick rego lat long height heading"
    while (process.canReadLine()) {
	QDateTime times = QDateTime::currentDateTime();
	QByteArray line = process.readLine();
	QString input(line);
	QStringList l = input.split((QLatin1Char(' ')));

	if (l.size() == 7) {
	    double latitude = l.at(3).toDouble();
	    double longitude =  l.at(4).toDouble();
	    double altitude = l.at(5).toDouble();
	    float heading = l.at(6).toFloat();
	    QGeoCoordinate coordinates(latitude, longitude, altitude);
	    updatedIdentity(l.at(0), l.at(1), l.at(2));
	    updatedPosition(times, coordinates);
	    updatedHeading(heading);
	} else {
	    stop();
	}
    }
}

class TestProximity: public QObject
{
    Q_OBJECT

private slots:
    void checkProximityWithoutOthers();
    void checkProximityWithOthers();
};

void TestProximity::checkProximityWithoutOthers()
{
    FlyingObjects all(true);
    QSignalSpy planeSpy(&all, SIGNAL(objectAppeared(FlyingObject*)));
    QSignalSpy alarmSpy(&all, SIGNAL(alarm()));

    TestAircraft central("./flying-object-central");
    central.start();
    QTest::qWait(100);

    // check that 0 aircraft appeared
    QCOMPARE(planeSpy.count(), 0);

    // check no proximity alerts seen
    QVERIFY(alarmSpy.count() == 0);

    central.stop();
}

void TestProximity::checkProximityWithOthers()
{
    FlyingObjects all(true);
    QSignalSpy planeSpy(&all, SIGNAL(objectAppeared(FlyingObject*)));
    QSignalSpy alarmSpy(&all, SIGNAL(alarm()));

    TestAircraft central("./flying-object-central");
    central.start();

    TestAircraft sally("./flying-object-sally");
    sally.start();

    TestAircraft kate("./flying-object-kate");
    kate.start();

    TestAircraft jim("./flying-object-jim");
    jim.start();

    QTest::qWait(100);

    // check that 4 aircraft appeared
    QCOMPARE(planeSpy.count(), 4);

    // check proximity alerts happened
    QVERIFY(alarmSpy.count() > 0);

    jim.stop();
    kate.stop();
    sally.stop();
    central.stop();
}

QTEST_MAIN(TestProximity)
#include "testproximity.moc"
