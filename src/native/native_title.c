/* SPDX-License-Identifier: GPL-2.0-or-later
 * Experimental ordinary-thread entry. No device execution has validated it.
 * All external stock imports go through explicit assembly bridges. */
#include <stdint.h>
#include "session_limits.h"
#include "interfaces.h"
#include "title_surface.h"
#include "display_backend.h"
extern void *title_stock_alloc(unsigned, unsigned);
extern unsigned title_stock_uptime(void);
extern void title_stock_sleep(unsigned);
extern void title_stock_flush(void*, void*, unsigned);
extern void title_stock_publish(void*);
extern uintptr_t title_current_sp(void);
extern void display_fatal(void) __attribute__((noreturn));
#include "native_diagnostics.h"
#include "timing_probe.h"
#include "timing_screen.h"
static uint32_t statistics_pixels[TITLE_SOURCE_WIDTH*TITLE_SOURCE_HEIGHT] __attribute__((aligned(64)));
volatile struct title_diagnostics title_diag;
volatile int native_engine_exit_status,native_input_cleanup_status;
volatile unsigned native_completion_reason;
unsigned native_demo_limit __attribute__((section(".data.demo_controls")))=SESSION_GAME_TICS;
extern int input_native_start(unsigned);
extern void input_native_step(unsigned);
extern void input_native_snapshot(unsigned[5]);
extern void input_native_stop(void);
volatile unsigned native_input_last_fault,native_input_samples;
unsigned native_timing_window;
unsigned native_input_before_stop_phase;
static unsigned input_started;
static const uint32_t *last_frame;
static uintptr_t pool_base;
/* Read only the stock constructor's existing sentinel. Never initialize or
 * switch this active VDK stack. This is an observation, not overflow prevention. */
static int observe_stack(void) {
    title_diag.stack_guard_ok=0;
    uintptr_t tcb=*(volatile uint32_t*)0xff802de4u;
    if ((tcb&3u) || tcb<0x104u || tcb>0x00a00000u-192u) return 0;
    uintptr_t object=tcb-4u;
    if (*(volatile uint32_t*)(object+192u)!=0x00fff1b0u) return 0;
    unsigned words=*(volatile uint32_t*)(object+0x2cu);
    uintptr_t top=*(volatile uint32_t*)(object+0x30u);
    if (words!=65581u || (top&3u) || top>0x00a00000u-4u || top<words*4u) return 0;
    uintptr_t bottom=top+4u-words*4u;
    if (bottom<0x100u || *(volatile uint32_t*)(tcb+0x14u)!=bottom) return 0;
    uintptr_t sp=title_current_sp();
    if (sp<bottom+32u || sp>top+4u) return 0;
    title_diag.stack_bottom=(unsigned)bottom; title_diag.stack_top=(unsigned)top;
    title_diag.stack_sp=(unsigned)sp;
    for (unsigned i=0;i<8u;i++) if (((volatile uint32_t*)bottom)[i]!=0xdeadbeefu) return 0;
    title_diag.stack_guard_ok=1;
    return 1;
}
static int valid_frame(const uint32_t *p) {
    uintptr_t a=(uintptr_t)p;
    return pool_base && a>=pool_base && !(a&3u) &&
           a<=pool_base+TITLE_POOL_BYTES-TITLE_SOURCE_BYTES;
}
static unsigned clock_read(void *context) {
    (void)context; unsigned now=title_stock_uptime();input_native_step(now);return now;
}
static void sleep_positive(void *context,unsigned n) {
    (void)context;
    if (n>=0x7fffffffu) adapter_unwind(3);
    while(n){unsigned chunk=n>5u?5u:n;title_diag.sleep_calls++;title_stock_sleep(chunk);input_native_step(title_stock_uptime());n-=chunk;}
}
static int keys_live(void *context,unsigned held[2]) {
    (void)context;unsigned s[5];unsigned now=title_stock_uptime();
    timing_probe_gap(TP_KEYS,now);
    if(!input_started){if(!input_native_start(now))adapter_unwind(3);input_started=1;}
    input_native_step(now);input_native_snapshot(s);
    {extern void adapter_request_input_stop(unsigned,unsigned);adapter_request_input_stop(s[2],s[3]);}
    held[0]=s[0];held[1]=s[1];native_input_last_fault=s[3];native_input_samples=s[4];return 1;
}
static int retain_frame(void *context,const uint32_t *p,unsigned w,unsigned h,
                        unsigned generation,unsigned job_mode) {
    (void)context;
    if (w!=TITLE_SOURCE_WIDTH || h!=TITLE_SOURCE_HEIGHT || job_mode!=2u || !valid_frame(p)) {
        adapter_unwind(3);
    }
    unsigned frame_now=title_stock_uptime();timing_probe_gap(TP_FRAME,frame_now);
    last_frame=p; title_diag.frames=generation;
    title_diag.frame=(unsigned)(uintptr_t)p;
    unsigned submit_begin=title_stock_uptime();
    int accepted=display_submit(p);
    timing_probe_span(accepted?TP_SUBMIT_OK:TP_SUBMIT_DROP,submit_begin,title_stock_uptime());
    if(accepted) title_diag.copied_frames++;
    /* Positive pacing keeps this experiment cooperative. It is not a measured
     * FPS guarantee; game tics and native rendering time remain separate. */
    unsigned sleep_begin=title_stock_uptime();
    /* Service a copied reply before the first yield, preserving batch progress. */
    input_native_step(sleep_begin);
    sleep_positive(context,20u);
    timing_probe_span(TP_SLEEP,sleep_begin,title_stock_uptime());
    return 0; /* Copied output: engine keeps its private BGRX buffer. */
}
static void trace_byte(void *context,unsigned ch) {
    (void)context;
    unsigned n=title_diag.trace_count;
    title_diag.trace[n&1023u]=(unsigned char)ch;
    title_diag.trace_count=n+1u;
}
static void retain_capture(void *context,const uint32_t *p,unsigned frame,
                           unsigned tic,unsigned job_mode) {
    (void)context;
    if (!valid_frame(p) || job_mode!=2u) adapter_unwind(3);
    if(tic==35u&&!native_timing_window){timing_probe_init();native_timing_window=35u;}
    last_frame=p; title_diag.capture_frame=frame; title_diag.capture_tic=tic;
}
static const struct adapter_api api={0,clock_read,sleep_positive,keys_live,
                                    retain_frame,trace_byte,retain_capture};
unsigned native_title_once(void) {
    if (title_diag.entered) return 9u;
    title_diag.entered=1; title_diag.stage=1;
    timing_probe_init();
    if (!observe_stack()) return title_diag.result=10;
    uintptr_t raw=(uintptr_t)title_stock_alloc(0,TITLE_RAW_BYTES);
    title_diag.raw=(unsigned)raw;
    /* Explicit heap0 ownership region, cold-trimmed before CRT registration.
     * This does not prove allocator internals, but rejects off-region returns. */
    if (raw<0x100u || raw>0x00a00000u-TITLE_RAW_BYTES) return title_diag.result=1;
    pool_base=title_pool_layout(raw,TITLE_RAW_BYTES);
    if (!pool_base) return title_diag.result=2;
    title_diag.pool=(unsigned)pool_base;
    unsigned char *pre=(unsigned char*)(pool_base-32u);
    unsigned char *post=(unsigned char*)(pool_base+TITLE_POOL_BYTES);
    for(unsigned i=0;i<32u;i++) pre[i]=post[i]=0xa5u;
    adapter_set_pool((unsigned char*)pool_base,TITLE_POOL_BYTES);
    adapter_bind(&api); mode=2;
    if(!native_demo_limit || native_demo_limit>SESSION_GAME_TICS) return title_diag.result=13;
    target_tics=native_demo_limit;
    display_start();
    title_diag.stage=2;
    extern int adapter_terminal_input(void);extern unsigned adapter_completion_reason(void);
    int result=doom_job();native_engine_exit_status=result;native_completion_reason=adapter_completion_reason();
    timing_probe_freeze();
    unsigned final_input[5];input_native_snapshot(final_input);
    native_input_before_stop_phase=final_input[2];
    input_native_stop();
    int input_cleanup=adapter_terminal_input();native_input_cleanup_status=input_cleanup;
    input_native_snapshot(final_input);
    native_input_last_fault=final_input[3];native_input_samples=final_input[4];
    title_diag.stage=3; title_diag.guard_ok=1;
    if (!observe_stack()) return title_diag.result=11;
    for(unsigned i=0;i<32u;i++) if(pre[i]!=0xa5u || post[i]!=0xa5u) title_diag.guard_ok=0;
    if (!title_diag.guard_ok) return title_diag.result=4;
    if (result!=0 || input_cleanup!=1) return title_diag.result=5;
    if (!valid_frame(last_frame) || !title_diag.frames ||
        title_diag.capture_frame!=title_diag.frames || (native_completion_reason<STOP_TARGET || native_completion_reason>STOP_INPUT_FAULT) ||
        (native_completion_reason==STOP_TARGET && title_diag.capture_tic<target_tics) ||
        (native_completion_reason==STOP_INPUT_BUDGET && native_input_before_stop_phase!=3u) ||
        (native_completion_reason==STOP_INPUT_FAULT && native_input_before_stop_phase!=4u && !native_input_last_fault))
        return title_diag.result=6;
    title_diag.stage=4;
    /* Request the exact final image again if it was dropped under pressure.
     * All buffers remain owned on timeout; no assumed hardware release. */
    unsigned begin=title_stock_uptime();
    while(!display_submit(last_frame)) {
        if((unsigned)(title_stock_uptime()-begin)>=2000u) return title_diag.result=12;
        title_stock_sleep(1u);
    }
    title_diag.copied_frames++;
    unsigned submitted=display_queue.submitted;
    title_diag.stage=5;
    begin=title_stock_uptime();
    while(display_queue.pending!=FQ_NONE || display_queue.published!=submitted) {
        if(display_queue.fault) display_fatal();
        if((unsigned)(title_stock_uptime()-begin)>=2000u) return title_diag.result=12;
        title_stock_sleep(1u);
    }
    title_diag.presented_frames=display_queue.published;
    if(!observe_stack()) return title_diag.result=11;
    title_diag.stage=6;
    unsigned input_result=(native_input_last_fault||native_input_before_stop_phase==4u)?16u:0u;
    /* Frozen thread-owned statistics use a separate private raw surface. The
     * queued display backend still reserves, converts and publishes a FREE
     * native surface. Neither raw game pixels nor ACTIVE output is overwritten. */
    title_diag.stage=7;
    if(!timing_screen(statistics_pixels,sizeof(statistics_pixels),&timing_diag,native_timing_window))return title_diag.result=17;
    if(!timing_screen_input(statistics_pixels,sizeof(statistics_pixels),native_input_last_fault,native_input_before_stop_phase))return title_diag.result=17;
    begin=title_stock_uptime();
    while(!display_submit(statistics_pixels)){
        if((unsigned)(title_stock_uptime()-begin)>=2000u)return title_diag.result=12;
        title_stock_sleep(1u);
    }
    title_diag.copied_frames++;
    submitted=display_queue.submitted;begin=title_stock_uptime();
    while(display_queue.pending!=FQ_NONE || display_queue.published!=submitted){
        if(display_queue.fault)display_fatal();
        if((unsigned)(title_stock_uptime()-begin)>=2000u)return title_diag.result=12;
        title_stock_sleep(1u);
    }
    title_diag.presented_frames=display_queue.published;

    if(!observe_stack()) return title_diag.result=11;
    title_diag.guard_ok=0;
    for(unsigned i=0;i<32u;i++) if(pre[i]!=0xa5u || post[i]!=0xa5u) return title_diag.result=4;
    title_diag.guard_ok=1;
    title_diag.stage=8;
    return title_diag.result=input_result;
}
