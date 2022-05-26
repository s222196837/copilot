#include "MyMetrics.h"

#define COPILOT_CLUSTER	42

pmAtomValue MyMetrics::unused;

MyMetrics::MyMetrics(): QObject(), enabled(false), item(0), mapping(0)
{
    metrics = mmv_stats_registry("copilot", COPILOT_CLUSTER, MMV_FLAG_PROCESS);

    if (metrics) {
	enabled = true;
    } else {
	fprintf(stderr, "%s: %s - %s\n",
		"mmv_stats_registry", "copilot", strerror(errno));
	memset(&unused, 0, sizeof(unused));
    }
}

MyMetrics::~MyMetrics()
{
    enabled = false;
    if (metrics)
	mmv_stats_free(metrics);
    metrics = NULL;
}

void
MyMetrics::add(const char *name, const char *help)
{
    if (enabled == false)
	return;

    // simple counter metrics only (no sets of values, no non-u64 types)
    if ((mmv_stats_add_metric(metrics, name, item++,
		    MMV_TYPE_U64, MMV_SEM_COUNTER,
		    MMV_UNITS(0,0,1,0,0,PM_COUNT_ONE), 0, help, NULL)) < 0) {
	fprintf(stderr, "%s: %s - %s\n",
		"mmv_stats_add_metric", "copilot", strerror(errno));
	enabled = false;
    }
}

void
MyMetrics::addf(const char *name, const char *help)
{
    if (enabled == false)
	return;

    // simple floating point metrics only (no sets, non-counter values)
    if ((mmv_stats_add_metric(metrics, name, item++,
		    MMV_TYPE_FLOAT, MMV_SEM_INSTANT,
		    MMV_UNITS(0,0,0,0,0,0), 0, help, NULL)) < 0) {
	fprintf(stderr, "%s: %s - %s\n",
		"mmv_stats_add_metric", "copilot", strerror(errno));
	enabled = false;
    }
}

void
MyMetrics::start(void)
{
    if (enabled == false)
	return;

    // system is unable to continue (e.g. filesystem access, no memory)
    if ((mapping = mmv_stats_start(metrics)) == NULL) {
	fprintf(stderr, "%s: %s - %s\n",
			"mmv_stats_start", "copilot", strerror(errno));
	enabled = false;
    }
}

uint64_t *
MyMetrics::map(const char *name)
{
    if (enabled == false)
	return &unused.ull;

    pmAtomValue	*value = mmv_lookup_value_desc(mapping, name, NULL);

    if (value == NULL) {
	fprintf(stderr, "%s: %s - %s\n",
			"mmv_lookup_value_desc", "copilot", strerror(errno));
	enabled = false;
	return NULL;
    }
    return &value->ull;
}

float *
MyMetrics::mapf(const char *name)
{
    if (enabled == false)
	return &unused.f;

    pmAtomValue	*value = mmv_lookup_value_desc(mapping, name, NULL);

    if (value == NULL) {
	fprintf(stderr, "%s: %s - %s\n",
			"mmv_lookup_value_desc", "copilot", strerror(errno));
	enabled = false;
	return NULL;
    }
    return &value->f;
}
