#pragma once
#include <stdint.h>
struct control_events { uint32_t held; unsigned read,count; unsigned short events[11]; };
/* Caller drains a batch before asking for another snapshot. */
int controls_update(struct control_events*,const uint32_t physical[2]);
int controls_pop(struct control_events*,int *pressed,unsigned char *key);
/* Call synchronously after the engine clears its key state at a level load.
 * Discard stale pending edges. Caller must invalidate its sample epoch too. */
int controls_engine_cleared(struct control_events*);
