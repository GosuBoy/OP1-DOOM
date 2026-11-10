#include "input_state.h"
#include "session_limits.h"
#define TOTAL_TICKS INPUT_TOTAL_TICKS
#define REPLY_TICKS 1000u
#define BATCH_REST_TICKS 20u
#define MAX_REQUESTS INPUT_MAX_REQUESTS
static void end(struct input_state*s,uint32_t phase,uint32_t fault){s->phase=phase;s->fault=fault;s->held[0]=s->held[1]=0;}
void input_init(struct input_state*s,uint32_t now){
 s->phase=IN_IDLE;s->fault=0;s->start=now;s->sent=now;s->last=now-BATCH_REST_TICKS;s->sequence=0;s->group=0;s->cycles=0;
 for(unsigned i=0;i<4;i++)s->staging[i]=0;
 s->held[0]=s->held[1]=0;
}
void input_service(struct input_state*s,uint32_t now){
 if(s->phase==IN_STOP||s->phase==IN_FAULT)return;
 if(s->phase>IN_READY||s->group>=4u||s->sequence>MAX_REQUESTS){end(s,IN_FAULT,IN_BAD_STATE);return;}
 if(now-s->start>=TOTAL_TICKS){end(s,IN_STOP,IN_OK);return;}
 if(s->phase==IN_WAIT&&now-s->sent>=REPLY_TICKS){end(s,IN_FAULT,IN_TIMEOUT);return;}
 if(s->phase==IN_READY){
  if(s->group==3u){s->held[0]=s->staging[0]|(s->staging[1]<<16);s->held[1]=s->staging[2]|(s->staging[3]<<16);s->cycles++;s->group=0;}
  else s->group++;
  s->phase=IN_IDLE;s->last=now;
 }
}
int input_begin(struct input_state*s,uint32_t now,int guard,uint32_t*address,uint32_t*tag){
 input_service(s,now);
 if(s->phase!=IN_IDLE||!guard)return 0;
 if(s->sequence==MAX_REQUESTS){end(s,IN_STOP,IN_OK);return 0;}
 /* Keep the former four five-tick waits as one pause between full scans. */
 if(s->group==0u&&s->sequence!=0u&&now-s->last<BATCH_REST_TICKS)return 0;
 s->sequence++;s->sent=now;s->phase=IN_WAIT;
 *address=0x23u+s->group;*tag=s->sequence;return 1;
}
void input_reply(struct input_state*s,uint32_t tag,int valid,uint16_t word){
 if(s->phase==IN_STOP||s->phase==IN_FAULT)return;
 if(s->phase!=IN_WAIT||s->group>=4u){end(s,IN_FAULT,IN_BAD_STATE);return;}
 if(tag!=s->sequence){end(s,IN_FAULT,IN_BAD_TAG);return;}
 if(!valid){end(s,IN_FAULT,IN_BAD_REPLY);return;}
 s->staging[s->group]=((uint32_t)word^65535u)&(s->group==3u?63u:65535u);
 s->phase=IN_READY;
}

void input_stop(struct input_state*s){if(s->phase!=IN_FAULT)end(s,IN_STOP,IN_OK);}
