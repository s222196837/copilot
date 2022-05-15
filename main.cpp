#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "MyMetrics.h"
#include "MySettings.h"
#include "FlyingObject.h"
#include "Transponder.h"
#include "Altimu10.h"
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

    bool debug = false;
    // optionally increase console verbosity
    for (int i = 0; i < argc; i++) {
	if (strcmp(argv[i], "-v") == 0 ||
	    strcmp(argv[i], "--debug") == 0 ||
	    strcmp(argv[i], "--verbose") == 0 ||
	    strcmp(argv[i], "--diagnostics") == 0)
	    debug = true;
    }

    QGuiApplication app(argc, argv);

    QFontDatabase fontDatabase;
    if (fontDatabase.addApplicationFont(":/fonts/myfontello.ttf") == -1)
	fprintf(stderr, "%s: Failed to load myfontello.ttf\n", "copilot");

    MyMetrics metrics;
    MySettings settings;
    GPS gps("copilot-gps", &metrics, debug);
    Altimu10 altimu10("copilot-altimu10", &metrics, debug);
    Transponder transponder(&metrics, &settings, debug);
    FlyingObject aircraft;

    // instrumentation is available from now
    metrics.start();

    // inject C++ objects into QML and load resources
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gps", &gps);
    engine.rootContext()->setContextProperty("altimu10", &altimu10);
    engine.rootContext()->setContextProperty("aircraft", &aircraft);
    engine.rootContext()->setContextProperty("settings", &settings);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // hardware bringup
    transponder.start();
    altimu10.start();
    gps.start();

    // main loop
    return app.exec();
}

#include "main.moc"
