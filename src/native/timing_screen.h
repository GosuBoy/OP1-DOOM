#pragma once
#include "timing_probe.h"
int timing_screen(uint32_t *pixels,unsigned bytes,const struct timing_probe *probe,unsigned window);

int timing_screen_input(uint32_t *pixels,unsigned bytes,unsigned fault,unsigned phase);
