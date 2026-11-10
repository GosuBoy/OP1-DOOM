#include "timing_screen.h"
static uint32_t pixels[64016];static struct timing_probe probe;
static void out(unsigned x){asm volatile("OUTC %0;"::"d"(x):"memory");}
static void word(unsigned x){for(unsigned i=0;i<4;i++)out((x>>(8*i))&255);}
static void check(int x,unsigned l){if(!x){word(0x4641494c);word(l);asm volatile("HLT;");}}
#define CHECK(x) check((x),__LINE__)
void test_events(void){
 for(unsigned i=0;i<64016;i++)pixels[i]=0xdeadbeef;
 for(unsigned i=0;i<7;i++){probe.stats[i].count=i==5?0:105;probe.stats[i].total=4200;probe.stats[i].minimum=1+i;probe.stats[i].maximum=10+i;}
 CHECK(!timing_screen(pixels+8,255999,&probe,35));CHECK(pixels[8]==0xdeadbeef);
 CHECK(timing_screen(pixels+8,256000,&probe,35));
 CHECK(!timing_screen_input(pixels+8,255999,0,1));
 CHECK(timing_screen_input(pixels+8,256000,0,1));
 CHECK(pixels[8+176*320+128]==0x40ff80u&&pixels[8+176*320+282]==0x40ff80u);
 for(unsigned i=0;i<8;i++){CHECK(pixels[i]==0xdeadbeef);CHECK(pixels[64008+i]==0xdeadbeef);}
 word(0x53435238);for(unsigned i=8;i<64008;i++)word(pixels[i]);
}
