#ifndef MYMETRICS_H
#define MYMETRICS_H

#include <QObject>
#include <pcp/pmapi.h>
#include <pcp/mmv_stats.h>

class MyMetrics : public QObject
{
    Q_OBJECT

public:
    MyMetrics();
    ~MyMetrics();

    // add() registers new metric definitions
    void add(const char *, const char *);

    // create memory mapped instrumentation file
    void start();

    // only map() metrics after start() - mmap offset
    uint64_t *map(const char *);

private:
    bool		enabled;
    int			item;	// next available metric itemID
    void		*mapping;	// base of mmap content
    mmv_registry_t	*metrics;	// table of all metrics
    uint64_t		unused;	// accessed if metrics disabled
};

#endif // MYMETRICS_H
