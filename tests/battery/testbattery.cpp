#include <QtTest/QtTest>
#include <QSignalSpy>
#include "Battery.h"

class TestBattery: public QObject
{
    Q_OBJECT

private slots:
    void parseInvalidData();
    void parseMissingData();
    void parseNoData();
    void parseValidData();
};

void TestBattery::parseValidData()
{
    Battery battery("./battery-good.sh");
    QSignalSpy statusSpy(&battery, SIGNAL(statusChanged()));
    QSignalSpy chargeSpy(&battery, SIGNAL(chargeChanged()));
    QSignalSpy poweredSpy(&battery, SIGNAL(poweredChanged()));
    battery.start();
    QTest::qWait(100);

    QVERIFY(battery.valid() == true);

    // verify values extracted from the final line
    QCOMPARE(battery.getCharge(), 42);

    // verify the expected signals were generated
    QVERIFY(statusSpy.count() > 0);
    QVERIFY(chargeSpy.count() > 0);
    QVERIFY(poweredSpy.count() > 0);
}

void TestBattery::parseInvalidData()
{
    Battery battery("./battery-bad.sh");
    QSignalSpy statusSpy(&battery, SIGNAL(statusChanged()));
    battery.start();
    QTest::qWait(100);

    QVERIFY(battery.valid() == false);
    QVERIFY(statusSpy.count() == 0);
}

void TestBattery::parseMissingData()
{
    Battery battery("./battery-short.sh");
    QSignalSpy chargeSpy(&battery, SIGNAL(chargeChanged()));
    battery.start();
    QTest::qWait(100);

    QVERIFY(battery.valid() == false);
    QVERIFY(chargeSpy.count() == 0);
}

void TestBattery::parseNoData()
{
    Battery battery("/dev/null");
    QSignalSpy poweredSpy(&battery, SIGNAL(poweredChanged()));
    battery.start();
    QTest::qWait(100);

    QVERIFY(battery.valid() == false);
    QVERIFY(poweredSpy.count() == 0);
}

QTEST_MAIN(TestBattery)
#include "testbattery.moc"
