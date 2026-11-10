#include "timing_probe.h"
struct timing_probe timing_diag;
static unsigned enabled;
void timing_probe_freeze(void){enabled=0;}
void timing_probe_init(void){enabled=1;for(unsigned i=0;i<TP_COUNT;i++){timing_reset(&timing_diag.stats[i]);timing_diag.previous[i]=timing_diag.valid[i]=timing_diag.rejected[i]=0;}}
void timing_probe_span(unsigned i,uint32_t a,uint32_t b){if(!enabled||i>=TP_COUNT)return;if(!timing_record(&timing_diag.stats[i],a,b)&&timing_diag.rejected[i]!=UINT32_MAX)timing_diag.rejected[i]++;}
void timing_probe_gap(unsigned i,uint32_t now){if(!enabled||i>=TP_COUNT)return;if(timing_diag.valid[i])timing_probe_span(i,timing_diag.previous[i],now);timing_diag.previous[i]=now;timing_diag.valid[i]=1;}
