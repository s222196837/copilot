#include <QGuiApplication>

#include "MyMetrics.h"
#include "MySettings.h"
#include "Transmitter.h"
#include "GPS.h"

int
main(int argc, char *argv[])
{
    QStringList identity;
    bool debug = false;

    QCoreApplication::setOrganizationName("testpilot");
    QCoreApplication::setApplicationName("testpilot");

    // optionally increase console verbosity
    for (int i = 0; i < argc; i++) {
	if (strcmp(argv[i], "-v") == 0 ||
	    strcmp(argv[i], "--debug") == 0 ||
	    strcmp(argv[i], "--verbose") == 0 ||
	    strcmp(argv[i], "--diagnostics") == 0)
	    debug = true;
	else if (i > 0)
	    identity.append(argv[i]);
    }

    if (identity.size() < 1) {
	fprintf(stderr, "%s: requires at least one argument (pilot name)\n",
			"testpilot");
	exit(1);
    }
    if (identity.size() < 2)
	identity.append(QString());
    if (identity.size() < 3)
	identity.append(QString());

    QGuiApplication app(argc, argv);
    MySettings settings(debug, false);
    settings.setTestsEnabled(true);

    // multicast networking bringup
    Transmitter transmitter(NULL, NULL, debug);
    transmitter.updatedIdentity(identity.at(0), identity.at(1), identity.at(2));
    transmitter.updatedSettings(&settings);
    transmitter.start();

    // main loop
    return app.exec();
}
