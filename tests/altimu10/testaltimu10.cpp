#include <QtTest/QtTest>
#include <QSignalSpy>
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
    QSignalSpy temperatureSpy(&altimu10, SIGNAL(temperatureChanged()));
    QSignalSpy pressureSpy(&altimu10, SIGNAL(pressureChanged()));
    QSignalSpy headingSpy(&altimu10, SIGNAL(updatedHeading(float)));
    QSignalSpy pitchSpy(&altimu10, SIGNAL(pitchChanged()));
    QSignalSpy rollSpy(&altimu10, SIGNAL(rollChanged()));
    QSignalSpy yawSpy(&altimu10, SIGNAL(yawChanged()));
    altimu10.start();
    QTest::qWait(100);

    QVERIFY(altimu10.valid() == true);

    // verify values extracted from the final line
    QCOMPARE(altimu10.getTemperature(), 21.148);
    QCOMPARE(altimu10.getPressure(), 1023.820);

    // verify the expected signals were generated
    QVERIFY(temperatureSpy.count() > 0);
    QVERIFY(pressureSpy.count() > 0);
    QVERIFY(headingSpy.count() > 0);
    QVERIFY(pitchSpy.count() > 0);
    QVERIFY(rollSpy.count() > 0);
    QVERIFY(yawSpy.count() > 0);
}

void TestAltimu10::parseInvalidData()
{
    Altimu10 altimu10("./altimu10-ahrs-bad.sh");
    QSignalSpy pitchSpy(&altimu10, SIGNAL(pitchChanged()));
    altimu10.start();
    QTest::qWait(100);

    QVERIFY(altimu10.valid() == false);
    QVERIFY(pitchSpy.count() == 0);
}

void TestAltimu10::parseMissingData()
{
    Altimu10 altimu10("./altimu10-ahrs-short.sh");
    QSignalSpy rollSpy(&altimu10, SIGNAL(rollChanged()));
    altimu10.start();
    QTest::qWait(100);

    QVERIFY(altimu10.valid() == false);
    QVERIFY(rollSpy.count() == 0);
}

void TestAltimu10::parseNoData()
{
    Altimu10 altimu10("/dev/null");
    QSignalSpy yawSpy(&altimu10, SIGNAL(yawChanged()));
    altimu10.start();
    QTest::qWait(100);

    QVERIFY(altimu10.valid() == false);
    QVERIFY(yawSpy.count() == 0);
}

QTEST_MAIN(TestAltimu10)
#include "testaltimu10.moc"
