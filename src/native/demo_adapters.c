/* SPDX-License-Identifier: GPL-2.0-or-later. Full engine fixed-layout adapters. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doomgeneric.h"
#include "doomkeys.h"
#include "doomstat.h"
#include "w_file.h"
#include "interfaces.h"
#include "control_events.h"
#include "session_limits.h"
#include "title_surface.h"
extern void episode1_bind_controls(void);
unsigned mode __attribute__((section(".data.fixture_controls")))=0,target_tics=350,force_guard_fault __attribute__((section(".data.fixture_controls")))=0,force_engine_fault_gt __attribute__((section(".data.fixture_controls")))=0;
unsigned char *heap_start,*heap_end;
static const struct adapter_api *api;
static unsigned env[48],armed,frames,reads,gen,publish_ok,publish_drop;
static unsigned char *second_frame;
static struct control_events controls;
static const pixel_t *last_complete_frame;
static unsigned last_complete_tic,completion_reason,input_stop_request;
static unsigned key_epoch=0xffffffff;
extern unsigned char wad_start[],wad_end[];static wad_file_t wad;
void adapter_bind(const struct adapter_api*a){api=a;}
void adapter_set_pool(unsigned char*p,unsigned n){heap_start=p;heap_end=p+n;}
void adapter_trace(unsigned c){api->trace_byte(api->context,c);}
void adapter_unwind(int code){if(!armed){for(;;);}module_longjmp(env,code+1);}
static wad_file_t *openwad(char*p){extern int wadname(const char*);if(!wadname(p))return 0;wad.length=wad_end-wad_start;return &wad;}
static void closewad(wad_file_t*w){}
static size_t readwad(wad_file_t*w,unsigned off,void*d,size_t n){if(w!=&wad||off>w->length||n>w->length-off)return 0;memcpy(d,wad_start+off,n);reads++;return n;}
wad_file_class_t stdc_wad_file={openwad,closewad,readwad};
void DG_Init(void){if(!DG_ScreenBuffer){puts("FRAME_ALLOC_FAIL");exit(1);}second_frame=malloc(TITLE_SOURCE_BYTES);if(!second_frame){puts("SECOND_FRAME_ALLOC_FAIL");exit(1);}wad.file_class=&stdc_wad_file;wad.mapped=0;wad.length=wad_end-wad_start;}
uint32_t DG_GetTicksMs(void){return api->clock_raw(api->context);}void DG_SleepMs(uint32_t ms){if(ms)api->wait_positive(api->context,ms);}void DG_SetWindowTitle(const char*s){printf("TITLE %s\n",s);}
/* Six physical mappings checked against the owner key-grid footage. */
int DG_GetKey(int*p,unsigned char*k){
 int ready=controls_pop(&controls,p,k);if(ready<0)adapter_unwind(3);if(ready)return 1;
 if(key_epoch==(unsigned)gametic)return 0;
 key_epoch=gametic;
 unsigned held[2]={0,0};if(!api->keys_held(api->context,held))return 0;
 uint32_t raw[2]={held[0],held[1]&0x003fffffu};
 if(controls_update(&controls,raw)!=1)adapter_unwind(3);
 ready=controls_pop(&controls,p,k);if(ready<0)adapter_unwind(3);return ready;
}
void DG_DrawFrame(void){frames++;last_complete_frame=DG_ScreenBuffer;last_complete_tic=gametic;if(force_engine_fault_gt&&(unsigned)gametic>=force_engine_fault_gt){puts("INJECTED_ENGINE_FAULT");exit(1);}unsigned capture_now=(mode==0?frames==20:mode==1?(unsigned)gametic>=target_tics:gametic==35||gametic==105||gametic==210||(unsigned)gametic>=target_tics);if(input_stop_request)capture_now=1;if(capture_now){unsigned h=2166136261u;unsigned char*p=(unsigned char*)DG_ScreenBuffer;for(unsigned i=0;i<TITLE_SOURCE_BYTES;i++)h=(h^p[i])*16777619u;printf("FRAME n=%u gt=%d gs=%d fnv=%08x reads=%u peak=%u live=%u\n",frames,gametic,gamestate,h,reads,heap_peak,heap_live);if(players[0].mo)printf("PLAYER gt=%d x=%d y=%d angle=%u health=%d ammo=%d buttons=%u forward=%d\n",gametic,players[0].mo->x,players[0].mo->y,players[0].mo->angle,players[0].health,players[0].ammo[0],players[0].cmd.buttons,players[0].cmd.forwardmove);api->capture(api->context,DG_ScreenBuffer,frames,gametic,mode);}
if(api->publish(api->context,DG_ScreenBuffer,TITLE_SOURCE_WIDTH,TITLE_SOURCE_HEIGHT,++gen,mode)){publish_ok++;unsigned char*p=(unsigned char*)DG_ScreenBuffer;DG_ScreenBuffer=(pixel_t*)second_frame;second_frame=p;}else publish_drop++;
if(input_stop_request){completion_reason=input_stop_request;printf("SESSION_INPUT_STOP %u\n",completion_reason);exit(0);}if((mode==0&&frames>=20)||(mode==1&&(unsigned)gametic>=target_tics)||(mode==2&&(unsigned)gametic>=target_tics)){completion_reason=STOP_TARGET;printf("PUBLICATION accepted=%u dropped=%u\n",publish_ok,publish_drop);exit(0);}if(frames>SESSION_MAX_FRAMES){completion_reason=STOP_FRAME_LIMIT;puts("FRAME_LIMIT");exit(1);}}
static unsigned le32(unsigned char*p){return p[0]|p[1]<<8|p[2]<<16|p[3]<<24;}
static void validate_wad(void){unsigned n=wad_end-wad_start;if(n<12||memcmp(wad_start,"IWAD",4)){puts("WAD_HEADER_FAIL");exit(1);}unsigned count=le32(wad_start+4),dir=le32(wad_start+8);if(count>n/16||dir>n||count*16>n-dir){puts("WAD_DIRECTORY_FAIL");exit(1);}for(unsigned i=0;i<count;i++){unsigned char*p=wad_start+dir+16*i;unsigned o=le32(p),z=le32(p+4);if(o>n||z>n-o){puts("WAD_LUMP_FAIL");exit(1);}}wad.length=n;unsigned char sentinel=0xa5;if(readwad(&wad,n+1,&sentinel,1)||readwad(&wad,n,&sentinel,1)||sentinel!=0xa5){puts("WAD_BOUNDS_FAIL");exit(1);}printf("WAD_VALID bytes=%u lumps=%u\n",n,count);}
int doom_job(void){int result=module_setjmp(env);if(result){armed=0;return result-1;}armed=1;episode1_bind_controls();validate_wad();singletics=1;
if(mode==0){char*a[]={"doomgeneric","-iwad","DOOM1.WAD","-nomouse","-nosound","-nogui"};doomgeneric_Create(6,a);}else if(mode==1){char*a[]={"doomgeneric","-iwad","DOOM1.WAD","-nomouse","-nosound","-nogui","-playdemo","demo1"};doomgeneric_Create(8,a);}else{char*a[]={"doomgeneric","-iwad","DOOM1.WAD","-nomouse","-nosound","-nogui","-warp","1","1","-skill","3"};doomgeneric_Create(11,a);}for(;;)doomgeneric_Tick();}

unsigned adapter_completion_reason(void){return completion_reason;}
void adapter_engine_keys_cleared(void){
 if(controls_engine_cleared(&controls)!=1)adapter_unwind(3);
 key_epoch=0xffffffffu;
}
int adapter_terminal_input(void){
 extern void G_AdapterClearKeys(void);
 G_AdapterClearKeys();key_epoch=0xffffffffu;
 return controls_engine_cleared(&controls);
}
void adapter_prepare_quit(void){
 completion_reason=STOP_QUIT;
 if(last_complete_frame&&frames)api->capture(api->context,last_complete_frame,frames,last_complete_tic,mode);
}
void adapter_request_input_stop(unsigned phase,unsigned fault){
 if(phase==3u||phase==4u){unsigned kind=(phase==4u||fault)?STOP_INPUT_FAULT:STOP_INPUT_BUDGET;
  if(kind>input_stop_request)input_stop_request=kind;
 }
}
