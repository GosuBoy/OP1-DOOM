/* Offline instrumentation component. No device integration yet. */
#pragma once
#include <stdint.h>
struct timing_stats { uint32_t count, total, minimum, maximum, overflows; };
void timing_reset(struct timing_stats *s);
/* Unsigned clock subtraction handles one wrap. Caller must bound each interval
   below half the clock range. Modulo differences at or above half the range
   are rejected, but elapsed whole wraps cannot be detected from two samples. */
int timing_record(struct timing_stats *s, uint32_t begin, uint32_t end);
