/* Offline draft. Native transport imports require independent ABI review. */
#include <stdint.h>
#include "input_state.h"
#include "timing_probe.h"
extern unsigned display_lock(void);
extern void display_unlock(unsigned);
extern void input_stock_request(unsigned address,unsigned tag);
static struct input_state keys;
static unsigned started;
static uint32_t rd32(uintptr_t p){return *(volatile uint32_t*)p;}
static unsigned rd8(uintptr_t p){return *(volatile uint8_t*)p;}
int input_native_start(unsigned now){
 unsigned mask=display_lock();
 if(started){display_unlock(mask);return 0;}
 input_init(&keys,now);started=1;display_unlock(mask);return 1;
}
void input_native_step(unsigned now){ unsigned mask=display_lock();
 unsigned old_cycles=keys.cycles;
 unsigned measure=started&&keys.phase!=IN_STOP&&keys.phase!=IN_FAULT;
 unsigned completed=0;
 if(started){
  uint32_t address,tag;
  uint32_t head=rd32(0x01dfe028),tail=rd32(0x01dfe02c);
  int idle=head<128u && tail==head && !rd8(0xff802c28) &&
   !rd8(0x01dfe030) && !rd8(0x01dfe031) &&
   !rd8(0x01dfe034) && !rd8(0x01dfe035) && !rd32(0xff802bdc);
  if(input_begin(&keys,now,idle,&address,&tag))input_stock_request(address,tag);
  completed=keys.cycles!=old_cycles;
 }
 display_unlock(mask);
 /* Thread-owned instrumentation stays outside the transport critical section. */
 if(measure)timing_probe_gap(TP_SERVICE,now);
 if(completed)timing_probe_gap(TP_SCAN,now);
}
void input_native_reply(const unsigned char *response,unsigned tag){
 unsigned mask=display_lock();
 if(started && keys.phase!=IN_STOP && keys.phase!=IN_FAULT){
  int valid=(uintptr_t)response==0xff802be8u &&
   *(volatile uint16_t*)0xff802bd0u==2u && *(volatile uint16_t*)0xff802bd4u==2u;
  uint16_t word=0;
  if(valid){const volatile unsigned char*p=response;word=(uint16_t)(p[0]|((unsigned)p[1]<<8));}
  input_reply(&keys,tag,valid,word);
 }
 display_unlock(mask);
}
void input_native_snapshot(unsigned out[5]){
 unsigned mask=display_lock();
 out[0]=keys.held[0];out[1]=keys.held[1];out[2]=keys.phase;out[3]=keys.fault;out[4]=keys.cycles;
 display_unlock(mask);
}

void input_native_stop(void){unsigned mask=display_lock();if(started)input_stop(&keys);display_unlock(mask);}
