#include <QDebug>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "FlyingObject.h"
#include "Transponder.h"
#include "Altimu10.h"
#include "GPS.h"

int
main(int argc, char *argv[])
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
    FlyingObject aircraft;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gps", &gps);
    engine.rootContext()->setContextProperty("altimu10", &altimu10);
    engine.rootContext()->setContextProperty("aircraft", &aircraft);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    gps.start();
    altimu10.start();

    return app.exec();
}

#include "main.moc"
