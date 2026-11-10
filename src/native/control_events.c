#include "control_events.h"
/* Eleven owner-selected physical bindings; see layout.json. */
static const unsigned char bits[11]={10,3,11,12,26,27,44,43,20,21,19};
static const unsigned char codes[11]={0xac,0xad,0xaf,0xae,0xa3,0xa2,0x1b,0x0d,0xf1,0xf0,0xb8};
int controls_update(struct control_events*s,const uint32_t physical[2]){
 if(s->read>s->count||s->count>11||s->held&~2047u)return -1;
 if(s->read!=s->count)return 0;
 uint32_t now=0;
 for(unsigned i=0;i<11;i++)if(physical[bits[i]/32]&(1u<<(bits[i]%32)))now|=1u<<i;
 s->read=s->count=0;
 for(unsigned i=0;i<11;i++)if((now^s->held)&(1u<<i))s->events[s->count++]=(unsigned short)(((now>>i&1u)<<8)|codes[i]);
 s->held=now;return 1;
}
int controls_pop(struct control_events*s,int*pressed,unsigned char*key){
 if(s->read>s->count||s->count>11)return -1;
 if(s->read==s->count)return 0;
 unsigned e=s->events[s->read++];*pressed=(e>>8)&1u;*key=(unsigned char)e;return 1;
}
int controls_engine_cleared(struct control_events*s){
 if(s->read>s->count||s->count>11||s->held&~2047u)return -1;
 s->held=0;s->read=s->count=0;
 return 1;
}
