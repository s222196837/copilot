#include "Buzzer.h"

#define FAST_BEEP_TIME	0.1	// short sharp beep indicating lift
#define LONG_BEEP_TIME	1.0	// 2 warning beeps, proximity alert

Buzzer::Buzzer(QString program, MyMetrics *registry, MySettings *config, bool debug):
	volume(60), enabled(true), fastBeep(false), longBeep(false),
	settings(config), diagnostics(debug), audibleAlarm(true),
	command(program)
{
    if ((metrics = registry) != NULL) {
	metrics->add("buzzer.errors", "count of failed buzzer commands");
	metrics->add("buzzer.count", "successfully sent buzzer commands");
    }

    if (config) {
	connect(config, &MySettings::proximityAudibleChanged,
			this, &Buzzer::updateAudibleAlarm);
    }

    connect(this, &QProcess::errorOccurred, this, &Buzzer::errorOccurred);
}

Buzzer::~Buzzer()
{
    if (state() != QProcess::NotRunning) {
	kill(); // self-termination signal
	waitForFinished(25); // msec units
    }
}

void
Buzzer::start()
{
    // prepare memory mapped metric pointers for live updating
    if (metrics) {
	errors = metrics->map("buzzer.errors");
	count = metrics->map("buzzer.count");
    } else {
	count = errors = &MyMetrics::unused.ull;
    }

    QProcess::start(command, QStringList());
}

void
Buzzer::errorOccurred(QProcess::ProcessError error)
{
    if (error != QProcess::ReadError)
	(*errors)++;
}

void
Buzzer::setVolume(int percent)
{
    if (percent > 0 && volume <= 100) {
	volume = percent;
	emit volumeChanged();
    }
}

void
Buzzer::setFastBeep(bool beep)
{
    fastBeep = beep;	// track most recent beep style
    longBeep = !beep;
    sendBeep(true);
}

void
Buzzer::setLongBeep(bool beep)
{
    if (audibleAlarm == false)
	return;
    longBeep = beep;	// track most recent beep style
    fastBeep = !beep;
    sendBeep(false);
}

void
Buzzer::updateAudibleAlarm()
{
    if (settings)
	audibleAlarm = settings->proximityAudible();
}

void
Buzzer::alarm()
{
    setLongBeep(true);
}

void
Buzzer::sendBeep(bool quick)
{
    char	buffer[64];
    size_t	i, size, pulse = quick? 1 : 3;
    int		level = quick? volume / 10.0 : volume;

    if (enabled == false)
	return;

    size = snprintf(buffer, sizeof(buffer), "%f %d\n",
		    quick? FAST_BEEP_TIME : LONG_BEEP_TIME, level);
    buffer[sizeof(buffer)-1] = '\0';

    for (i = 0; i < pulse; i++)
	writeData(buffer, size);

    if (diagnostics)
	fprintf(stderr, "BUZZER: %s", buffer);

    (*count)++;

    emit soundEmitted();
}
