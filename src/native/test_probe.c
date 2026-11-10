#include "timing_probe.h"
#include <assert.h>
#include <string.h>
int main(void){
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
 return 0;
}
