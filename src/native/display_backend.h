/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once
#include <stdint.h>
#include "frame_queue.h"
extern struct frame_queue display_queue;
int display_start(void);
int display_submit(const uint32_t*);
void display_irq_hook(void);
