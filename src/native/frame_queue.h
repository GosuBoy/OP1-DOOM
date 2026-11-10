/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once
#include <stdint.h>
enum { FQ_FREE, FQ_FILLING, FQ_PENDING, FQ_ACTIVE, FQ_NONE=2 };
struct frame_queue {
    volatile uint32_t enabled, fault, active, pending, states[2];
    volatile uint32_t submitted, published, dropped;
};
/* Thread operations require the caller to save/disable/restore its incoming
 * interrupt mask. They must not be called on an active queue without that lock.
 * IRQ operations require exclusive callback execution while DMA_RUN is clear.
 * No function touches pixels or hardware; the caller owns both buffers forever. */
int fq_init(struct frame_queue*,unsigned initially_active);
unsigned fq_reserve(struct frame_queue*);
int fq_cancel(struct frame_queue*,unsigned slot);
/* Commit only after the complete surface was converted, flushed and synchronized. */
int fq_commit(struct frame_queue*,unsigned slot);
unsigned fq_irq_choice(struct frame_queue*,unsigned shared_source,unsigned dma_running);
/* Call only AFTER updating/synchronizing shared source and ready in the same IRQ.
 * The calling thread must remain excluded until the stock IRQ tail has returned. */
int fq_irq_complete(struct frame_queue*,unsigned published_source);
