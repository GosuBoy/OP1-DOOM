/* Derived from Doom R_DrawColumn, Copyright (C) 1993-1996 id Software, Inc.
 * Copyright (C) 2005-2014 Simon Howard.
 * GPL version 2 or later; see THIRD_PARTY_LICENSES/doom-GPL-2.0.txt.
 * Experimental scalar column renderer: no integration or device approval.
 */
#include <stdint.h>
extern int dc_yh, dc_yl, dc_x, dc_iscale, dc_texturemid, centery;
extern unsigned char *ylookup[], *dc_source, *dc_colormap;
extern int columnofs[];
extern void I_Error(char *, ...) __attribute__((noreturn));

void R_DrawColumn(void)
{
    int count = dc_yh - dc_yl;
    if (count < 0) return;
    /* Preserve the enabled original RANGECHECK and its error arguments. */
    if ((unsigned)dc_x >= 440u || dc_yl < 0 || dc_yh >= 200)
        I_Error("R_DrawColumn: %i to %i at %i", dc_yl, dc_yh, dc_x);

    /* These objects are stable during one draw and cannot alias the globals.
     * Integer address progression avoids forming an unused out-of-array
     * pointer after the last output byte. Only valid pixel addresses are used.
     */
    uintptr_t dest = (uintptr_t)(ylookup[dc_yl] + columnofs[dc_x]);
    const unsigned char *source = dc_source, *map = dc_colormap;
    uint32_t step = (uint32_t)dc_iscale;
    uint32_t frac = (uint32_t)dc_texturemid +
                    ((uint32_t)dc_yl - (uint32_t)centery) * step;
    do {
        *(unsigned char *)dest = map[source[(frac >> 16) & 127u]];
        dest += 440u;
        frac += step;
    } while (count--);
}
