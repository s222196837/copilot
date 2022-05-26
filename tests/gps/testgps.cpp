#include <QtTest/QtTest>
#include <QSignalSpy>
#include "GPS.h"

class TestGPS : public QObject
{
    Q_OBJECT

private slots:
    void parseValidData();
    void parseInvalidData();
    void parseMissingData();
    void parseNoData();
};

void TestGPS::parseValidData()
{
    GPS gps("./gpscat-good.sh");
    QSignalSpy spy0(&gps, SIGNAL(updatedPosition(QDateTime, QGeoCoordinate)));
    QSignalSpy spy1(&gps, SIGNAL(positionChanged()));
    gps.start();
    QTest::qWait(100);

    QVERIFY(gps.valid() == true);

    // verify values extracted from the final line
    QCOMPARE(gps.getPosition().latitude(), -38.312945);
    QCOMPARE(gps.getPosition().longitude(), 144.289195);

    QVERIFY(spy0.count() > 0);
    QVERIFY(spy1.count() > 0);
}

void TestGPS::parseInvalidData()
{
    GPS gps("./gpscat-bad.sh");
    QSignalSpy spy0(&gps, SIGNAL(updatedPosition(QDateTime, QGeoCoordinate)));
    QSignalSpy spy1(&gps, SIGNAL(positionChanged()));
    gps.start();
    QTest::qWait(100);

    QVERIFY(gps.valid() == false);
    QVERIFY(spy0.count() == 0);
    QVERIFY(spy1.count() == 0);
}

void TestGPS::parseMissingData()
{
    GPS gps("./gpscat-short.sh");
    QSignalSpy spy0(&gps, SIGNAL(updatedPosition(QDateTime, QGeoCoordinate)));
    QSignalSpy spy1(&gps, SIGNAL(positionChanged()));
    gps.start();
    QTest::qWait(100);

    QVERIFY(gps.valid() == false);
    QVERIFY(spy0.count() == 0);
    QVERIFY(spy1.count() == 0);
}

void TestGPS::parseNoData()
{
    GPS gps("/dev/null");
    QSignalSpy spy0(&gps, SIGNAL(updatedPosition(QDateTime, QGeoCoordinate)));
    QSignalSpy spy1(&gps, SIGNAL(positionChanged()));
    gps.start();
    QTest::qWait(100);

    QVERIFY(gps.valid() == false);
    QVERIFY(spy0.count() == 0);
    QVERIFY(spy1.count() == 0);
}

QTEST_MAIN(TestGPS)
#include "testgps.moc"
