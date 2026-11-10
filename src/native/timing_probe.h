#pragma once
#include "timing_stats.h"
enum { TP_SERVICE,TP_SCAN,TP_KEYS,TP_FRAME,TP_SUBMIT_OK,TP_SUBMIT_DROP,TP_SLEEP,TP_COUNT };
struct timing_probe {struct timing_stats stats[TP_COUNT];uint32_t previous[TP_COUNT],valid[TP_COUNT],rejected[TP_COUNT];};
extern struct timing_probe timing_diag;
void timing_probe_init(void);
void timing_probe_freeze(void);
void timing_probe_gap(unsigned which,uint32_t now);
void timing_probe_span(unsigned which,uint32_t begin,uint32_t end);
