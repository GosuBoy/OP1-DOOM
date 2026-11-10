/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "title_surface.h"

static const uint16_t converter_column_offsets[374] = {
    0u, 1u, 2u, 4u, 5u, 6u, 7u, 8u, 9u, 11u, 12u, 13u,
    14u, 15u, 16u, 18u, 19u, 20u, 21u, 22u, 24u, 25u, 26u, 27u,
    28u, 29u, 31u, 32u, 33u, 34u, 35u, 36u, 38u, 39u, 40u, 41u,
    42u, 44u, 45u, 46u, 47u, 48u, 49u, 51u, 52u, 53u, 54u, 55u,
    56u, 58u, 59u, 60u, 61u, 62u, 64u, 65u, 66u, 67u, 68u, 69u,
    71u, 72u, 73u, 74u, 75u, 77u, 78u, 79u, 80u, 81u, 82u, 84u,
    85u, 86u, 87u, 88u, 89u, 91u, 92u, 93u, 94u, 95u, 97u, 98u,
    99u, 100u, 101u, 102u, 104u, 105u, 106u, 107u, 108u, 109u, 111u, 112u,
    113u, 114u, 115u, 117u, 118u, 119u, 120u, 121u, 122u, 124u, 125u, 126u,
    127u, 128u, 129u, 131u, 132u, 133u, 134u, 135u, 137u, 138u, 139u, 140u,
    141u, 142u, 144u, 145u, 146u, 147u, 148u, 149u, 151u, 152u, 153u, 154u,
    155u, 157u, 158u, 159u, 160u, 161u, 162u, 164u, 165u, 166u, 167u, 168u,
    169u, 171u, 172u, 173u, 174u, 175u, 177u, 178u, 179u, 180u, 181u, 182u,
    184u, 185u, 186u, 187u, 188u, 189u, 191u, 192u, 193u, 194u, 195u, 197u,
    198u, 199u, 200u, 201u, 202u, 204u, 205u, 206u, 207u, 208u, 209u, 211u,
    212u, 213u, 214u, 215u, 217u, 218u, 219u, 220u, 221u, 222u, 224u, 225u,
    226u, 227u, 228u, 230u, 231u, 232u, 233u, 234u, 235u, 237u, 238u, 239u,
    240u, 241u, 242u, 244u, 245u, 246u, 247u, 248u, 250u, 251u, 252u, 253u,
    254u, 255u, 257u, 258u, 259u, 260u, 261u, 262u, 264u, 265u, 266u, 267u,
    268u, 270u, 271u, 272u, 273u, 274u, 275u, 277u, 278u, 279u, 280u, 281u,
    282u, 284u, 285u, 286u, 287u, 288u, 290u, 291u, 292u, 293u, 294u, 295u,
    297u, 298u, 299u, 300u, 301u, 302u, 304u, 305u, 306u, 307u, 308u, 310u,
    311u, 312u, 313u, 314u, 315u, 317u, 318u, 319u, 320u, 321u, 322u, 324u,
    325u, 326u, 327u, 328u, 330u, 331u, 332u, 333u, 334u, 335u, 337u, 338u,
    339u, 340u, 341u, 342u, 344u, 345u, 346u, 347u, 348u, 350u, 351u, 352u,
    353u, 354u, 355u, 357u, 358u, 359u, 360u, 361u, 362u, 364u, 365u, 366u,
    367u, 368u, 370u, 371u, 372u, 373u, 374u, 375u, 377u, 378u, 379u, 380u,
    381u, 383u, 384u, 385u, 386u, 387u, 388u, 390u, 391u, 392u, 393u, 394u,
    395u, 397u, 398u, 399u, 400u, 401u, 403u, 404u, 405u, 406u, 407u, 408u,
    410u, 411u, 412u, 413u, 414u, 415u, 417u, 418u, 419u, 420u, 421u, 423u,
    424u, 425u, 426u, 427u, 428u, 430u, 431u, 432u, 433u, 434u, 435u, 437u,
    438u, 439u,
};

static const uint32_t converter_row_offsets[170] = {
    0u, 440u, 880u, 1760u, 2200u, 2640u, 3080u, 3520u, 3960u, 4840u, 5280u, 5720u,
    6160u, 6600u, 7040u, 7920u, 8360u, 8800u, 9240u, 9680u, 10560u, 11000u, 11440u, 11880u,
    12320u, 12760u, 13640u, 14080u, 14520u, 14960u, 15400u, 16280u, 16720u, 17160u, 17600u, 18040u,
    18480u, 19360u, 19800u, 20240u, 20680u, 21120u, 21560u, 22440u, 22880u, 23320u, 23760u, 24200u,
    25080u, 25520u, 25960u, 26400u, 26840u, 27280u, 28160u, 28600u, 29040u, 29480u, 29920u, 30360u,
    31240u, 31680u, 32120u, 32560u, 33000u, 33880u, 34320u, 34760u, 35200u, 35640u, 36080u, 36960u,
    37400u, 37840u, 38280u, 38720u, 39160u, 40040u, 40480u, 40920u, 41360u, 41800u, 42680u, 43120u,
    43560u, 44000u, 44440u, 44880u, 45760u, 46200u, 46640u, 47080u, 47520u, 48400u, 48840u, 49280u,
    49720u, 50160u, 50600u, 51480u, 51920u, 52360u, 52800u, 53240u, 53680u, 54560u, 55000u, 55440u,
    55880u, 56320u, 57200u, 57640u, 58080u, 58520u, 58960u, 59400u, 60280u, 60720u, 61160u, 61600u,
    62040u, 62480u, 63360u, 63800u, 64240u, 64680u, 65120u, 66000u, 66440u, 66880u, 67320u, 67760u,
    68200u, 69080u, 69520u, 69960u, 70400u, 70840u, 71280u, 72160u, 72600u, 73040u, 73480u, 73920u,
    74800u, 75240u, 75680u, 76120u, 76560u, 77000u, 77880u, 78320u, 78760u, 79200u, 79640u, 80520u,
    80960u, 81400u, 81840u, 82280u, 82720u, 83600u, 84040u, 84480u, 84920u, 85360u, 85800u, 86680u,
    87120u, 87560u,
};

uintptr_t title_pool_layout(uintptr_t raw, unsigned bytes) {
    uintptr_t top = ~(uintptr_t)0;
    if (!raw || bytes < TITLE_RAW_BYTES || raw > top - bytes) return 0;
    uintptr_t pool = (raw + 95u) & ~(uintptr_t)63u;
    if (pool < raw + 32u || pool > raw + bytes - TITLE_POOL_BYTES - 32u) return 0;
    return pool;
}
int title_surface_convert(const uint32_t *source, unsigned source_bytes,
                          uint16_t *surface, unsigned surface_bytes) {
    uintptr_t s=(uintptr_t)source, d=(uintptr_t)surface, top=~(uintptr_t)0;
    if (!s || !d || (s & 3u) || (d & 1u) ||
        source_bytes != TITLE_SOURCE_BYTES || surface_bytes != TITLE_SURFACE_BYTES ||
        s > top-source_bytes || d > top-surface_bytes) return 0;
    if (s < d+surface_bytes && d < s+source_bytes) return 0;
    /* Proportional 374x170 fit at logical (1,0), within the unchanged
     * 376x188 transfer. Endpoint-aware nearest sampling retains source corners.
     * Constant tables avoid per-pixel division; geometry is a hardware proposal. */
    for (unsigned x=0;x<1u;x++) {
        uint16_t *left=surface+(375u-x)*256u;
        uint16_t *right=surface+x*256u;
        for (unsigned y=0;y<188u;y++) left[y]=right[y]=0;
    }
    /* Clear the unused lower strip inside the content columns. No writes
     * outside the existing 376x188 transfer rectangle or into padding. */
    for (unsigned x=0;x<374u;x++) {
        uint16_t *column=surface+(374u-x)*256u;
        for (unsigned y=170u;y<188u;y++) column[y]=0;
    }
    /* Flatten the two column loops. The pixel index sequence remains the same,
     * but arithmetic and memory operations are grouped within each pair. */
    for (unsigned by=0;by<170u;by+=8u) {
        unsigned rows=170u-by;
        if(rows>8u)rows=8u;
        for (unsigned x=0;x<374u;x++) {
            const uint32_t *column=source+converter_column_offsets[x];
            uint16_t *out=surface+(374u-x)*256u+by;
            const uint32_t *row=converter_row_offsets+by;
            /* Tiles have eight rows, except the final two-row tile.
             * Pair arithmetic is unsigned and uses two 16-bit stores so
             * valid destinations with only two-byte alignment stay valid. */
            for (unsigned y=0;y<rows;y+=2u) {
                uint32_t p0=column[row[y]], p1=column[row[y+1u]];
                uint32_t rg,gb;
#ifdef __bfin__
                /* Each asm is one PACK instruction. Inputs are read before
                 * its output is written; PACK does not alter status bits. */
                __asm__("%0 = PACK(%1.H, %2.H);" : "=d"(rg) : "d"(p1), "d"(p0));
                __asm__("%0 = PACK(%1.L, %2.L);" : "=d"(gb) : "d"(p1), "d"(p0));
#else
                rg=(p1&0xffff0000u)|(p0>>16);
                gb=(p1<<16)|(p0&0xffffu);
#endif
                uint32_t packed=((rg&0x00f800f8u)<<8)|
                                ((gb&0xfc00fc00u)>>5)|
                                ((gb&0x00f800f8u)>>3);
                out[y]=(uint16_t)packed;
                out[y+1u]=(uint16_t)(packed>>16);
            }
        }
    }
    return 1;
}
