#include "timing_stats.h"
void timing_reset(struct timing_stats*s){s->count=0;s->total=0;s->minimum=UINT32_MAX;s->maximum=0;s->overflows=0;}
int timing_record(struct timing_stats*s,uint32_t begin,uint32_t end){
 uint32_t d=end-begin;
 if(d>=0x80000000u)return 0;
 if(s->count==UINT32_MAX){if(s->overflows!=UINT32_MAX)s->overflows++;return 0;}
 s->count++;
 if(d<s->minimum)s->minimum=d;
 if(d>s->maximum)s->maximum=d;
 if(UINT32_MAX-s->total<d){s->total=UINT32_MAX;if(s->overflows!=UINT32_MAX)s->overflows++;}
 else s->total+=d;
 return 1;
}
