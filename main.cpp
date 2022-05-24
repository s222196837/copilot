#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "MyMetrics.h"
#include "MySettings.h"
#include "FlyingObjects.h"
#include "Transmitter.h"
#include "Receiver.h"
#include "Altimu10.h"
#include "Battery.h"
#include "Buzzer.h"
#include "GPS.h"

int
main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setOrganizationName("copilot");
    QCoreApplication::setApplicationName("copilot");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    bool debug = false, tests = false;
    // optionally increase console verbosity
    for (int i = 0; i < argc; i++) {
	if (strcmp(argv[i], "-v") == 0 ||
	    strcmp(argv[i], "--debug") == 0 ||
	    strcmp(argv[i], "--verbose") == 0 ||
	    strcmp(argv[i], "--diagnostics") == 0)
	    debug = true;
	if (strcmp(argv[i], "--test") == 0 ||
	    strcmp(argv[i], "--tests") == 0 ||
	    strcmp(argv[i], "--testing") == 0 ||
	    strcmp(argv[i], "--test-pattern") == 0)
	    tests = true;
    }

    QGuiApplication app(argc, argv);

    QFontDatabase fontDatabase;
    if (fontDatabase.addApplicationFont(":/fonts/myfontello.ttf") == -1)
	fprintf(stderr, "%s: Failed to load myfontello.ttf\n", "copilot");

    MyMetrics metrics;
    MySettings settings(debug, tests);
    GPS gps("copilot-gps", &metrics, debug);
    Buzzer buzzer("copilot-buzzer", &metrics, &settings, debug);
    Battery battery("copilot-battery", &metrics, debug);
    Altimu10 altimu10("copilot-altimu10", &metrics, debug);
    Transmitter transmitter(&metrics, &settings, debug);
    Receiver receiver(&metrics, &settings, debug);

    // hook up the hardware to the proxity checking module
    FlyingObjects gaggle(debug);
    gaggle.connect(&gps, &GPS::updatedPosition,
            &gaggle, &FlyingObjects::proximityCheckByCoordinate);
    gaggle.connect(&settings, &MySettings::updatedProximityDistance,
            &gaggle, &FlyingObjects::updatedProximityDistance);
    gaggle.connect(&receiver, &Receiver::updateFlyingObject,
            &gaggle, &FlyingObjects::updateFlyingObject);
    gaggle.connect(&gaggle, &FlyingObjects::alarm, &buzzer, &Buzzer::alarm);

    // instrumentation is available from now
    metrics.start();

    // inject C++ objects into QML and load resources
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gps", &gps);
    engine.rootContext()->setContextProperty("buzzer", &buzzer);
    engine.rootContext()->setContextProperty("battery", &battery);
    engine.rootContext()->setContextProperty("altimu10", &altimu10);
    engine.rootContext()->setContextProperty("settings", &settings);
    engine.rootContext()->setContextProperty("gaggle", &gaggle);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // hardware bringup
    altimu10.start();
    battery.start();
    buzzer.start();
    gps.start();

    // multicast networking bringup
    receiver.setFilter(transmitter.device());
    transmitter.connect(&gps, &GPS::updatedPosition,
	    &transmitter, &Transmitter::updatedPosition);
    transmitter.connect(&altimu10, &Altimu10::updatedHeading,
	    &transmitter, &Transmitter::updatedHeading);
    transmitter.start();
    receiver.start();

    // main loop
    return app.exec();
}
