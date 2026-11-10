/* Pure bounded diagnostic state machine. Callers serialize all access. */
#pragma once
#include <stdint.h>
enum { IN_IDLE, IN_WAIT, IN_READY, IN_STOP, IN_FAULT };
enum { IN_OK, IN_BAD_REPLY, IN_BAD_TAG, IN_TIMEOUT, IN_BAD_STATE };
struct input_state {
 uint32_t phase,fault,start,sent,last,sequence,group,cycles;
 uint32_t staging[4],held[2];
};
void input_init(struct input_state*,uint32_t now);
/* service consumes a completed reply or stops on deadline. */
void input_service(struct input_state*,uint32_t now);
/* guard=1 means transport idle/queue empty/flags valid under same lock. */
int input_begin(struct input_state*,uint32_t now,int guard,uint32_t *address,uint32_t *tag);
/* word is already copied from a validated two-byte response; no pointer retained. */
void input_reply(struct input_state*,uint32_t tag,int valid,uint16_t word);

void input_stop(struct input_state*);
