#include <QtTest/QtTest>
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

    QVERIFY(gps.valid() == true);
    // verify values extracted from the final line
    QCOMPARE(gps.getPosition().latitude(), 38.18);
    QCOMPARE(gps.getPosition().longitude(), 144.17);
}

void TestGPS::parseInvalidData()
{
    GPS gps("./gpscat-bad.sh");

    QVERIFY(gps.valid() == false);
}

void TestGPS::parseMissingData()
{
    GPS gps("./gpscat-short.sh");

    QVERIFY(gps.valid() == false);
}

void TestGPS::parseNoData()
{
    GPS gps("/dev/null");

    QVERIFY(gps.valid() == false);
}

QTEST_MAIN(TestGPS)
#include "testgps.moc"
