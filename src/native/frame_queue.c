/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "frame_queue.h"
static int valid(struct frame_queue *q) {
    if (!q || !q->enabled || q->fault) return 0;
    unsigned a=q->active,p=q->pending;
    if (a>1u || p>FQ_NONE || q->states[a]!=FQ_ACTIVE) goto bad;
    unsigned o=1u-a;
    if (q->states[o]>FQ_PENDING) goto bad;
    if (p<FQ_NONE) {
        if (p==a || q->states[p]!=FQ_PENDING) goto bad;
    } else if(q->states[o]==FQ_PENDING) goto bad;
    return 1;
bad:
    q->fault=1;
    return 0;
}
int fq_init(struct frame_queue *q,unsigned a) {
    if (!q || a>1u) return 0;
    q->enabled=0; q->fault=0; q->active=a; q->pending=FQ_NONE;
    q->states[a]=FQ_ACTIVE; q->states[1u-a]=FQ_FREE;
    q->submitted=q->published=q->dropped=0;
    q->enabled=1;
    return 1;
}
unsigned fq_reserve(struct frame_queue *q) {
    if (!valid(q)) return FQ_NONE;
    unsigned o=1u-q->active;
    if(q->pending!=FQ_NONE || q->states[o]!=FQ_FREE) {q->dropped++;return FQ_NONE;}
    q->states[o]=FQ_FILLING;
    return o;
}
int fq_cancel(struct frame_queue *q,unsigned i) {
    if (!valid(q) || i>1u || q->states[i]!=FQ_FILLING) return 0;
    q->states[i]=FQ_FREE;
    return 1;
}
int fq_commit(struct frame_queue *q,unsigned i) {
    if (!valid(q) || i>1u || q->states[i]!=FQ_FILLING || q->pending!=FQ_NONE) return 0;
    q->states[i]=FQ_PENDING;
    q->pending=i;
    q->submitted++;
    return 1;
}
unsigned fq_irq_choice(struct frame_queue *q,unsigned source,unsigned running) {
    if (!q || !q->enabled || running) return FQ_NONE;
    if (!valid(q)) return FQ_NONE;
    if (source!=q->active) {q->fault=1;return FQ_NONE;}
    return q->pending;
}
int fq_irq_complete(struct frame_queue *q,unsigned source) {
    if (!valid(q) || source>1u || source!=q->pending) return 0;
    unsigned old=q->active;
    q->states[source]=FQ_ACTIVE;
    q->pending=FQ_NONE;
    q->active=source;
    q->states[old]=FQ_FREE;
    q->published++;
    return 1;
}
