/* SPDX-License-Identifier: GPL-2.0-or-later. Tests only: substituted stock
 * services, a software GPIO-opportunity pump and sampled fake DMA ownership. */
#include <stdint.h>
#include "title_surface.h"
#include "display_backend.h"
#include "native_diagnostics.h"
#include "timing_probe.h"
extern unsigned native_title_once(void);
extern unsigned native_timing_window,native_input_before_stop_phase;
#define CONTROL __attribute__((section(".data.sim_controls")))
volatile unsigned sim_fail_alloc CONTROL=0,sim_bad_guard CONTROL=0,sim_bad_stack CONTROL=0;
volatile unsigned sim_no_irq CONTROL=0,sim_wrong_source CONTROL=0,sim_dma_error CONTROL=0;
volatile unsigned sim_bad_stack_meta CONTROL=0,sim_raw CONTROL=0x10010;
volatile unsigned sim_status=0,sim_ticks=0,sim_failure=0,sim_flushes=0,sim_irqs=0;
static unsigned due,complete_at,samples[32],checks,entered;
static uintptr_t held;
extern int gametic;
extern unsigned gamekeydown[256];
static unsigned sim_final_keys;
extern void input_native_reply(const unsigned char*,unsigned);
extern void input_native_snapshot(unsigned[5]);
volatile unsigned sim_input_mode CONTROL=0,sim_hold_lo CONTROL=0,sim_hold_hi CONTROL=0;
static unsigned input_requests,input_tag,input_address,input_due,input_pending,input_seen[2],input_delayed;
__attribute__((section(".mock_input"))) void sim_input_request(unsigned address,unsigned selector,unsigned count,unsigned callback,unsigned tag){
 if(selector!=18||count!=2||callback!=(unsigned)(uintptr_t)input_native_reply||address<0x23||address>0x26||input_pending)sim_failure=20;
 input_requests++;input_tag=tag;input_address=address;input_due=sim_ticks+1;input_pending=1;
 *(volatile unsigned char*)0xff802c28u=1;*(volatile unsigned*)0xff802bdcu=callback;
}
static void input_pump(void){
 unsigned snap[5];input_native_snapshot(snap);input_seen[0]|=snap[0];input_seen[1]|=snap[1];
 if(input_pending && sim_ticks>=input_due && sim_input_mode!=1){
  unsigned group=input_address-0x23,lo=0,hi=0;
  if(gametic>=35 && gametic<105){lo=sim_hold_lo;hi=sim_hold_hi;}
  unsigned held=group<2?lo>>(group*16):hi>>((group-2)*16);
  *(volatile unsigned short*)0xff802be8u=(unsigned short)(~held);
  *(volatile unsigned short*)0xff802bd0u=sim_input_mode==3?1:2;
  *(volatile unsigned short*)0xff802bd4u=2;
  *(volatile unsigned char*)0x01dfe034u=1;
  input_native_reply((sim_input_mode==2||(sim_input_mode==4&&gametic>=105))?0:(void*)0xff802be8u,input_tag);
  *(volatile unsigned char*)0x01dfe034u=0;*(volatile unsigned char*)0xff802c28u=0;
  *(volatile unsigned*)0xff802bdcu=0;input_pending=0;
  if(sim_input_mode==5&&!input_delayed){input_delayed=1;sim_ticks+=1500;}
 }
}
static void out(unsigned c){asm volatile("OUTC %0;"::"d"(c):"memory");}
static void word(unsigned n){for(unsigned i=0;i<4;i++)out((n>>(8*i))&255u);}
static unsigned sample(unsigned i){unsigned x=(i*13u)%376u,y=(i*7u)%188u;return ((uint16_t*)held)[(375u-x)*256u+y];}
static void check(void){if((sim_status&8u)&&held){for(unsigned i=0;i<32;i++)if(sample(i)!=samples[i])sim_failure=1;checks++;}}
static void pump(void){
 check();
 if((sim_status&8u)&&sim_ticks>=complete_at)sim_status=0;
 if(sim_no_irq || title_diag.stage>=7u || entered || sim_ticks<due)return;
 entered=1;due=sim_ticks+17u;sim_irqs++;
 if(sim_dma_error && display_queue.enabled)sim_status=2u;
 if(sim_wrong_source && display_queue.enabled)*(volatile unsigned*)0x018b64b0u=0x02000004u;
 display_irq_hook();
 /* Models an allowed forced repeat or ordinary post-hook transfer. This is
  * not execution of the stock IRQ tail or its actual GPIO event source. */
 if(!sim_status){
  uintptr_t a=*(volatile unsigned*)0x018b64b0u;
  if(a!=0x02000000u && a!=0x03000000u)sim_failure=2;
  else {
   if(display_queue.enabled && display_queue.states[a==0x03000000u]!=FQ_ACTIVE)sim_failure=3;
   held=a;for(unsigned i=0;i<32;i++)samples[i]=sample(i);
   sim_status=8u;complete_at=sim_ticks+10u;
   *(volatile unsigned char*)0x018b64adu=0;
  }
 }
 entered=0;
}
void *sim_alloc(unsigned ordinal,unsigned n){if(ordinal||n!=TITLE_RAW_BYTES)sim_failure=4;return sim_fail_alloc?0:(void*)(uintptr_t)sim_raw;}
unsigned sim_uptime(void){
 if(sim_bad_guard&&title_diag.pool)((unsigned char*)(uintptr_t)title_diag.pool)[-1]=0;
 if(sim_bad_stack==1u)*(unsigned*)0x008fff4cu=0;
 if(sim_bad_stack==2u)*(unsigned*)0x0095002cu=0x4000u;
 sim_ticks++;pump();input_pump();return sim_ticks;
}
void sim_sleep(unsigned n){if(!n||n>=0x7fffffffu)sim_failure=5;sim_ticks+=n;pump();input_pump();}
void sim_flush(void *a,void *z,unsigned mode){
 uintptr_t p=(uintptr_t)a;
 if((p!=0x02000000u&&p!=0x03000000u)||(uintptr_t)z!=p+0x2ffe0u||mode)sim_failure=6;
 if(display_queue.states[p==0x03000000u]!=FQ_FILLING)sim_failure=7;
 check();sim_flushes++;
}
void sim_fatal(void) __attribute__((noreturn));
void sim_fatal(void){
 out('F');out('6');out('0');out('\n');word(title_diag.stage);word(display_queue.fault);
 word(sim_failure);word(sim_status);word(*(volatile unsigned*)0x018b64b0u);
 asm volatile("HLT;");for(;;){}
}
unsigned sim_test(void){
 *(volatile unsigned*)0xff802de4u=0x00950004u;
 *(volatile unsigned*)0x009500c0u=0x00fff1b0u;
 *(volatile unsigned*)0x0095002cu=sim_bad_stack_meta?0x4000u:65581u;
 *(volatile unsigned*)0x00950030u=0x0093fffcu;
 *(volatile unsigned*)0x00950018u=0x008fff4cu;
 *(volatile unsigned*)0x018b64b0u=0x02000000u;
 *(volatile unsigned char*)0x018b64adu=0;
 for(unsigned i=0;i<TITLE_SURFACE_BYTES;i++){
  ((unsigned char*)0x02000000u)[i]=0xa5;
  ((unsigned char*)0x03000000u)[i]=0x5a;
 }
 unsigned result=native_title_once();check();
 const unsigned codes[6]={0xac,0xad,0xaf,0xae,0xa3,0xa2};
 for(unsigned i=0;i<6;i++)if(gamekeydown[codes[i]])sim_final_keys|=1u<<i;
 if(sim_final_keys)sim_failure=21;
 out('D');out('6');out('0');out('\n');
 word(result);word(title_diag.stage);word(title_diag.frames);word(title_diag.capture_tic);
 word(title_diag.guard_ok);word(title_diag.stack_guard_ok);word(display_queue.submitted);
 word(display_queue.published);word(display_queue.dropped);word(display_queue.pending);
 word(display_queue.active);word(sim_flushes);word(sim_irqs);word(checks);word(sim_failure);
 word(*(volatile unsigned*)0x018b64b0u);word(native_title_once());
 unsigned snap[5];input_native_snapshot(snap);word(input_requests);word(input_seen[0]);word(input_seen[1]);word(snap[2]);word(snap[3]);word(snap[4]);word(title_diag.trace_count);word(sim_final_keys);
 if(!result || result==16){
  for(unsigned i=0;i<TITLE_SOURCE_BYTES;i++)out(((unsigned char*)(uintptr_t)title_diag.frame)[i]);
  uintptr_t src=*(volatile unsigned*)0x018b64b0u;
  for(unsigned i=0;i<TITLE_SURFACE_BYTES;i++)out(((unsigned char*)src)[i]);
 }
 for(unsigned i=0;i<1024;i++)out(title_diag.trace[i]);
 for(unsigned i=0;i<sizeof(timing_diag);i++)out(((unsigned char*)&timing_diag)[i]);
 word(native_timing_window);word(native_input_before_stop_phase);
 return result;
}
