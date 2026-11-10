#include "timing_probe.h"
static void word(unsigned x){for(unsigned i=0;i<4;i++){unsigned b=(x>>(8*i))&255;asm volatile("OUTC %0;"::"d"(b):"memory");}}
static void check(int x,unsigned line){if(!x){word(0x4641494c);word(line);asm volatile("HLT;");}}
#define assert(x) check((x),__LINE__)
static int memcmp(const void*a,const void*b,unsigned n){const unsigned char*x=a,*y=b;for(unsigned i=0;i<n;i++)if(x[i]!=y[i])return 1;return 0;}
void *memcpy(void*a,const void*b,unsigned n){unsigned char*x=a;const unsigned char*y=b;for(unsigned i=0;i<n;i++)x[i]=y[i];return a;}

void test_events(void){
 timing_probe_init();timing_probe_gap(TP_SERVICE,0xfffffff0u);
 assert(timing_diag.stats[TP_SERVICE].count==0);
 timing_probe_gap(TP_SERVICE,0x10u);
 assert(timing_diag.stats[TP_SERVICE].count==1&&timing_diag.stats[TP_SERVICE].total==32);
 timing_probe_span(TP_FRAME,0,0x80000000u);
 assert(timing_diag.rejected[TP_FRAME]==1&&!timing_diag.stats[TP_FRAME].count);
 struct timing_probe old=timing_diag;
 timing_probe_gap(TP_COUNT,10);timing_probe_span(0xffffffffu,0,1);
 assert(memcmp(&old,&timing_diag,sizeof old)==0);
 timing_probe_freeze();timing_probe_gap(TP_SERVICE,200);timing_probe_span(TP_FRAME,0,1);
 assert(memcmp(&old,&timing_diag,sizeof old)==0);
 timing_probe_init();assert(timing_diag.stats[TP_SERVICE].count==0&&timing_diag.valid[TP_SERVICE]==0&&timing_diag.rejected[TP_FRAME]==0);
 word(0x50524f38);
}
