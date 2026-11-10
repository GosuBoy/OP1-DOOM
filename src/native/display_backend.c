/* SPDX-License-Identifier: GPL-2.0-or-later. Draft native backend, offline only. */
#include "display_backend.h"
#include "title_surface.h"
extern unsigned display_lock(void);
extern void display_unlock(unsigned);
extern void display_sync(void);
extern void display_fatal(void) __attribute__((noreturn));
extern unsigned display_dma_status(void);
extern uintptr_t display_shared_source(void);
extern void title_stock_flush(void*,void*,unsigned);
extern void title_stock_publish(void*);
struct frame_queue display_queue;
static uintptr_t address(unsigned i) {return i ? 0x03000000u : 0x02000000u;}
static unsigned index_of(uintptr_t p) {
    if(p==0x02000000u)return 0;
    if(p==0x03000000u)return 1;
    return FQ_NONE;
}
int display_start(void) {
    unsigned mask=display_lock();
    /* Before this activation the bootstrap has published only surface0. */
    if(display_queue.enabled || display_shared_source()!=0x02000000u) display_fatal();
    int ok=fq_init(&display_queue,0);
    if(!ok)display_fatal();
    display_sync();
    display_unlock(mask);
    return 1;
}
int display_submit(const uint32_t *p) {
    unsigned mask=display_lock();
    unsigned slot=fq_reserve(&display_queue);
    if(display_queue.fault)display_fatal();
    display_unlock(mask);
    if(slot==FQ_NONE)return 0;
    if(!title_surface_convert(p,TITLE_SOURCE_BYTES,(uint16_t*)address(slot),TITLE_SURFACE_BYTES)) {
        mask=display_lock();
        if(!fq_cancel(&display_queue,slot) || display_queue.fault)display_fatal();
        display_unlock(mask);
        return 0;
    }
    title_stock_flush((void*)address(slot),(void*)(address(slot)+0x2ffe0u),0);
    display_sync();
    mask=display_lock();
    if(!fq_commit(&display_queue,slot) || display_queue.fault)display_fatal();
    display_sync();
    display_unlock(mask);
    return 1;
}
void display_irq_hook(void) {
    if(!display_queue.enabled)return;
    unsigned status=display_dma_status();
    if(status&8u)return;
    /* A DMA error or inconsistent source/ownership is terminal. Do not let the
     * stock tail start another transfer or resume an interrupted converter. */
    if(status&2u)display_fatal();
    unsigned source=index_of(display_shared_source());
    unsigned slot=fq_irq_choice(&display_queue,source,0);
    if(display_queue.fault)display_fatal();
    if(slot==FQ_NONE)return;
    title_stock_publish((void*)address(slot));
    if(!fq_irq_complete(&display_queue,slot) || display_queue.fault)display_fatal();
    display_sync();
}
