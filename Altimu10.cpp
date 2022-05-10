#include <QTextStream>
#include "Altimu10.h"

Altimu10::Altimu10(QString program): count(0)
{
    connect(this, &QProcess::readyReadStandardOutput, this, &Altimu10::tryRead);
    command = program;
}

Altimu10::~Altimu10()
{
    if (state() != QProcess::NotRunning) {
	kill();
	waitForFinished(20);
    }
}

void
Altimu10::tryRead()
{
    while (canReadLine()) {
	QByteArray bytes = readLine();

	if (parse(bytes.constData()) == true) {
	    fprintf(stderr, "ALTIMU10: %s", (const char *)bytes.constData());
	    count++;
	}
    }
}

bool
Altimu10::parse(const char *line)
{
    int n = sscanf(line, "%f %f %f %f %f %f %f %f %f %f %f\n",
		&velocity[0], &velocity[1], &velocity[2],
		&acceleration[0], &acceleration[1], &acceleration[2],
		&magnetic_field[0], &magnetic_field[1], &magnetic_field[2],
		&pressure, &temperature);
    return n == 11;
}
