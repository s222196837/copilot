#include <QtTest/QtTest>
#include "Battery.h"

class TestBattery: public QObject
{
    Q_OBJECT

private slots:
    void parseValidData();
    void parseInvalidData();
    void parseMissingData();
    void parseNoData();
};

void TestBattery::parseValidData()
{
    Battery battery("./battery-good.sh");

    QVERIFY(battery.valid() == true);
    // verify values extracted from the final line
    QCOMPARE(battery.getCharge(), 42);
}

void TestBattery::parseInvalidData()
{
    Battery battery("./battery-bad.sh");

    QVERIFY(battery.valid() == false);
}

void TestBattery::parseMissingData()
{
    Battery battery("./battery-short.sh");

    QVERIFY(battery.valid() == false);
}

void TestBattery::parseNoData()
{
    Battery battery("/dev/null");

    QVERIFY(battery.valid() == false);
}

QTEST_MAIN(TestBattery)
#include "testbattery.moc"
