/* Diagnostic pixels only; caller owns this complete 320x200 BGRX surface. */
#include "timing_screen.h"
#include "title_surface.h"
static const unsigned char font[19][5]={
 {7,5,5,5,7},{2,6,2,2,7},{7,1,7,4,7},{7,1,7,1,7},
 {5,5,7,1,1},{7,4,7,1,7},{7,4,7,5,7},{7,1,1,1,1},
 {7,5,7,5,7},{7,5,7,1,7},{2,5,7,5,5},{6,5,6,5,6},
 {7,4,4,4,7},{6,5,5,5,6},{7,4,6,4,7},{7,4,6,4,4},
 {5,7,7,7,5},{5,5,2,5,5},{0,0,7,0,0}};
static void glyph(uint32_t*p,unsigned x,unsigned y,unsigned n,uint32_t color){
 for(unsigned row=0;row<5;row++)for(unsigned col=0;col<3;col++)if(font[n][row]&(4u>>col))
  for(unsigned a=0;a<2;a++)for(unsigned b=0;b<2;b++)p[(y+row*2+a)*TITLE_SOURCE_WIDTH+x+60u+col*2+b]=color;
}
static void hex(uint32_t*p,unsigned x,unsigned y,uint32_t v,uint32_t color){for(unsigned i=0;i<8;i++)glyph(p,x+i*8,y,(v>>(28-i*4))&15u,color);}
int timing_screen(uint32_t*p,unsigned bytes,const struct timing_probe*t,unsigned window){
 if(!p||!t||bytes!=TITLE_SOURCE_BYTES)return 0;
 for(unsigned i=0;i<TITLE_SOURCE_WIDTH*TITLE_SOURCE_HEIGHT;i++)p[i]=0;
 glyph(p,32,4,12,0xffffffu);glyph(p,128,4,16,0xffffffu);glyph(p,224,4,17,0xffffffu);
 for(unsigned i=0;i<TP_COUNT;i++){
  const struct timing_stats*s=&t->stats[i];unsigned y=24+i*20;
  uint32_t color=t->rejected[i]||s->overflows?0xff3030u:!s->count?0xffff00u:0x40ff80u;
  glyph(p,8,y,i,color);hex(p,32,y,s->count,color);
  if(s->count){hex(p,128,y,s->minimum,color);hex(p,224,y,s->maximum,color);}
  else{for(unsigned j=0;j<8;j++){glyph(p,128+j*8,y,18,color);glyph(p,224+j*8,y,18,color);}}
 }
 hex(p,32,176,window,window==35u?0x40ff80u:0xff3030u);
 return 1;
}

/* Separate footer: retained fault code and phase observed before explicit stop. */
int timing_screen_input(uint32_t*p,unsigned bytes,unsigned fault,unsigned phase){
 if(!p||bytes!=TITLE_SOURCE_BYTES)return 0;
 hex(p,128,176,fault,fault?0xff3030u:0x40ff80u);
 hex(p,224,176,phase,phase<3u?0x40ff80u:phase==3u?0xffff00u:0xff3030u);
 return 1;
}
