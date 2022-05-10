#include <QtTest/QtTest>
#include "Altimu10.h"

class TestAltimu10: public QObject
{
    Q_OBJECT

private slots:
    void parseValidData();
    void parseInvalidData();
    void parseMissingData();
    void parseNoData();
};

void TestAltimu10::parseValidData()
{
    Altimu10 altimu10("./altimu10-ahrs-good.sh");

    QVERIFY(altimu10.valid() == true);
    // verify values extracted from the final line
    QCOMPARE(altimu10.getTemperature(), 21.148);
    QCOMPARE(altimu10.getPressure(), 1023.820);
}

void TestAltimu10::parseInvalidData()
{
    Altimu10 altimu10("./altimu10-ahrs-bad.sh");

    QVERIFY(altimu10.valid() == false);
}

void TestAltimu10::parseMissingData()
{
    Altimu10 altimu10("./altimu10-ahrs-short.sh");

    QVERIFY(altimu10.valid() == false);
}

void TestAltimu10::parseNoData()
{
    Altimu10 altimu10("/dev/null");

    QVERIFY(altimu10.valid() == false);
}

QTEST_MAIN(TestAltimu10)
#include "testaltimu10.moc"
