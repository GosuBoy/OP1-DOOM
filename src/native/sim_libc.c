/* SPDX-License-Identifier: GPL-2.0-or-later; simulator-only freestanding runtime. */
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
extern unsigned char *heap_start,*heap_end;extern unsigned char wad_start[],wad_end[];extern void adapter_trace(unsigned);extern void adapter_unwind(int) __attribute__((noreturn));
int errno;
void outbyte(unsigned x){adapter_trace(x);}
void exit(int code){printf("\nJOB_EXIT %d\n",code);adapter_unwind(code);}
void abort(void){exit(2);}
void *memcpy(void*d,const void*s,size_t n){unsigned char*a=d;const unsigned char*b=s;while(n--)*a++=*b++;return d;}
void *memset(void*d,int c,size_t n){unsigned char*a=d;while(n--)*a++=c;return d;}
void *memmove(void*d,const void*s,size_t n){unsigned char*a=d;const unsigned char*b=s;if(a>b){a+=n;b+=n;while(n--)*--a=*--b;}else memcpy(d,s,n);return d;}
int memcmp(const void*a,const void*b,size_t n){const unsigned char*x=a,*y=b;while(n--){if(*x!=*y)return *x-*y;x++;y++;}return 0;}
size_t strlen(const char*s){const char*p=s;while(*p)p++;return p-s;}
int strcmp(const char*a,const char*b){while(*a&&*a==*b){a++;b++;}return (unsigned char)*a-(unsigned char)*b;}
int strncmp(const char*a,const char*b,size_t n){while(n--){if(*a!=*b||!*a)return (unsigned char)*a-(unsigned char)*b;a++;b++;}return 0;}
int tolower(int c){return c>='A'&&c<='Z'?c+32:c;} int toupper(int c){return c>='a'&&c<='z'?c-32:c;}
int isdigit(int c){return c>='0'&&c<='9';} int isalpha(int c){return tolower(c)>='a'&&tolower(c)<='z';} int isalnum(int c){return isalpha(c)||isdigit(c);} int isspace(int c){return c==' '||(c>=9&&c<=13);}int isprint(int c){return c>=32&&c<127;}int isupper(int c){return c>='A'&&c<='Z';}int islower(int c){return c>='a'&&c<='z';}int isxdigit(int c){return isdigit(c)||(tolower(c)>='a'&&tolower(c)<='f');}
int strncasecmp(const char*a,const char*b,size_t n){while(n--){int x=tolower((unsigned char)*a++),y=tolower((unsigned char)*b++);if(x!=y||!x)return x-y;}return 0;}int strcasecmp(const char*a,const char*b){return strncasecmp(a,b,(size_t)-1);}
char *strcpy(char*d,const char*s){char*p=d;while((*p++=*s++));return d;}char *strncpy(char*d,const char*s,size_t n){char*p=d;while(n--){*p++=*s;if(*s)s++;}return d;}char *strcat(char*d,const char*s){strcpy(d+strlen(d),s);return d;}char *strchr(const char*s,int c){do{if(*s==c)return (char*)s;}while(*s++);return 0;}char *strrchr(const char*s,int c){const char*p=0;do{if(*s==c)p=s;}while(*s++);return (char*)p;}char *strstr(const char*s,const char*q){size_t n=strlen(q);for(;*s;s++)if(!strncmp(s,q,n))return(char*)s;return n?0:(char*)s;}
typedef struct Block{unsigned size,used;struct Block*next;unsigned pad;} Block;static Block *head;unsigned heap_peak,heap_live;
static unsigned accounting_blocks,accounting_peak_blocks,accounting_peak_total,accounting_min_free=~0u,accounting_requested_live,accounting_requested_peak;
static void account_pool(void){unsigned capacity=heap_end-heap_start,used=heap_live+accounting_blocks*sizeof(Block);if(used>capacity)abort();if(accounting_blocks>accounting_peak_blocks)accounting_peak_blocks=accounting_blocks;if(used>accounting_peak_total)accounting_peak_total=used;if(capacity-used<accounting_min_free)accounting_min_free=capacity-used;if(accounting_requested_live>accounting_requested_peak)accounting_requested_peak=accounting_requested_live;}

void *malloc(size_t n){size_t requested=n;if(n>0x1000000-31)return 0;n=(n+7)&~7u;if(!n)n=8;if(!head){head=(Block*)heap_start;head->size=heap_end-heap_start-sizeof(Block);head->used=0;head->next=0;accounting_blocks=1;account_pool();}for(Block*b=head;b;b=b->next)if(!b->used&&b->size>=n){if(b->size>=n+sizeof(Block)+8){Block*q=(Block*)((char*)(b+1)+n);q->size=b->size-n-sizeof(Block);q->used=0;q->next=b->next;b->next=q;b->size=n;accounting_blocks++;}b->used=1;b->pad=requested;heap_live+=b->size;if(heap_live>heap_peak)heap_peak=heap_live;accounting_requested_live+=requested;account_pool();return b+1;}return 0;}
void free(void*p){if(!p)return;Block*b=(Block*)p-1;if(!b->used)abort();b->used=0;heap_live-=b->size;accounting_requested_live-=b->pad;account_pool();for(b=head;b&&b->next;){if(!b->used&&!b->next->used){b->size+=sizeof(Block)+b->next->size;b->next=b->next->next;accounting_blocks--;}else b=b->next;}account_pool();}
void *calloc(size_t n,size_t z){if(z&&n>(size_t)-1/z)return 0;size_t k=n*z;void*p=malloc(k);if(p)memset(p,0,k);return p;}void *realloc(void*p,size_t n){if(!p)return malloc(n);if(!n){free(p);return 0;}Block*b=(Block*)p-1;if(b->size>=n)return p;void*q=malloc(n);if(q){memcpy(q,p,b->size);free(p);}return q;}char *strdup(const char*s){char*p=malloc(strlen(s)+1);if(p)strcpy(p,s);return p;}
unsigned long strtoul(const char*s,char**e,int base){while(isspace(*s))s++;int neg=*s=='-';if(*s=='-'||*s=='+')s++;if(!base)base=(*s=='0'?8:10);if(*s=='0'&&tolower(s[1])=='x'&&(base==16||base==8)){base=16;s+=2;}unsigned n=0;while(*s){int v=isdigit(*s)?*s-'0':tolower(*s)-'a'+10;if(v<0||v>=base)break;n=n*base+v;s++;}if(e)*e=(char*)s;return neg?-n:n;}long strtol(const char*s,char**e,int b){return strtoul(s,e,b);}int atoi(const char*s){return strtol(s,0,10);}int abs(int x){return x<0?-x:x;}char *getenv(const char*s){return 0;}char *strerror(int e){return "simulator unavailable file";}
static inline unsigned clz_nonzero(unsigned v) {
#ifdef __bfin__
    return (v >> 31) ? 0u : (unsigned)__builtin_bfin_norm_fr1x32((int)v) + 1u;
#else
    return (unsigned)__builtin_clz(v);
#endif
}
unsigned __udivsi3(unsigned a, unsigned b) {
    if (!b) abort();
    if (a < b) return 0;
    if (b == 1u) return a;
    if (!(b & (b - 1u))) return a >> (31u - clz_nonzero(b));
    /* a and b are nonzero here. Alignment cannot discard a high divisor bit:
     * msb(b) + shift == msb(a), at most31. */
    unsigned shift=clz_nonzero(b)-clz_nonzero(a);
    unsigned divisor=b << shift, q=0;
    do {
        q <<= 1;
        if (a >= divisor) { a -= divisor; q |= 1u; }
        divisor >>= 1;
    } while (shift--);
    return q;
}

unsigned __umodsi3(unsigned a,unsigned b){return a-__udivsi3(a,b)*b;}extern unsigned __udivsi3(unsigned,unsigned);
static inline unsigned magnitude(int v) {
    if ((unsigned)v == 0x80000000u) return 0x80000000u;
    return v < 0 ? 0u-(unsigned)v : (unsigned)v;
}
int __divsi3(int a, int b) {
    unsigned q=__udivsi3(magnitude(a),magnitude(b));
    return (int)(((unsigned)a ^ (unsigned)b) >> 31 ? 0u-q : q);
}

int __modsi3(int a,int b){return a-__divsi3(a,b)*b;}
unsigned long long __udivdi3(unsigned long long a,unsigned long long b){if(!b)abort();unsigned long long q=0,r=0;for(int i=63;i>=0;i--){unsigned high=r>>63;r=(r<<1)|((a>>i)&1);if(high||r>=b){r-=b;q|=1ull<<i;}}return q;}long long __divdi3(long long a,long long b){unsigned long long q=__udivdi3(a<0?0ull-a:a,b<0?0ull-b:b);return (a^b)<0?-q:q;}unsigned long long __umoddi3(unsigned long long a,unsigned long long b){return a-__udivdi3(a,b)*b;}long long __moddi3(long long a,long long b){return a-__divdi3(a,b)*b;}
static FILE streams[3];FILE*stdin=&streams[0],*stdout=&streams[1],*stderr=&streams[2];static FILE wadfile;
int wadname(const char*s){const char*p=strrchr(s,'/');return !strcasecmp(p?p+1:s,"doom1.wad");}
FILE*fopen(const char*p,const char*m){if(m[0]!='r'||!wadname(p)){errno=ENOENT;return 0;}wadfile.pos=0;wadfile.eof=0;return &wadfile;}int fclose(FILE*f){return 0;}size_t fread(void*p,size_t s,size_t n,FILE*f){unsigned len=wad_end-wad_start;if(!s)return 0;if(f!=&wadfile||f->pos>len)return 0;size_t count=(len-f->pos)/s;if(n>count)n=count;memcpy(p,wad_start+f->pos,s*n);f->pos+=s*n;f->eof=f->pos==len;return n;}int fseek(FILE*f,long off,int w){if(f!=&wadfile)return -1;long p=off+(w==SEEK_CUR?f->pos:w==SEEK_END?wad_end-wad_start:0);if(p<0||(unsigned)p>(unsigned)(wad_end-wad_start))return -1;f->pos=p;f->eof=0;return 0;}long ftell(FILE*f){return f->pos;}int fgetc(FILE*f){unsigned char c;return fread(&c,1,1,f)?c:EOF;}char*fgets(char*s,int n,FILE*f){int i=0,c;while(i<n-1&&(c=fgetc(f))!=EOF){s[i++]=c;if(c=='\n')break;}s[i]=0;return i?s:0;}int feof(FILE*f){return f->eof;}int fflush(FILE*f){return 0;}size_t fwrite(const void*p,size_t z,size_t n,FILE*f){if(f!=stdout&&f!=stderr)return 0;const unsigned char*s=p;for(size_t i=0;i<z*n;i++)outbyte(s[i]);return n;}int putchar(int c){outbyte(c);return c;}int puts(const char*s){while(*s)outbyte(*s++);outbyte(10);return 0;}void setbuf(FILE*f,char*b){}int remove(const char*s){return -1;}int rename(const char*a,const char*b){return -1;}int mkdir(const char*s,int m){return -1;}int isatty(int f){return 1;}
static void emit(char*d,size_t cap,unsigned*pos,char c){if(d){if(*pos+1<cap)d[*pos]=c;}else outbyte((unsigned char)c);(*pos)++;}
int vsnprintf(char*d,size_t cap,const char*f,va_list ap){unsigned pos=0;while(*f){if(*f!='%'){emit(d,cap,&pos,*f++);continue;}f++;if(*f=='%'){emit(d,cap,&pos,*f++);continue;}int left=0,zero=0,width=0,prec=-1,len=0;if(*f=='-'){left=1;f++;}if(*f=='0'){zero=1;f++;}while(isdigit(*f)){width=width*10+*f++-'0';}if(*f=='.'){f++;prec=0;if(*f=='*'){prec=va_arg(ap,int);f++;}else while(isdigit(*f))prec=prec*10+*f++-'0';}if(*f=='l'){len=1;f++;if(*f=='l'){len=2;f++;}}else if(*f=='z'){len=1;f++;}char spec=*f++;char buf[70],*s=buf;int n=0,neg=0;if(spec=='s'){s=va_arg(ap,char*);if(!s)s="(null)";n=strlen(s);if(prec>=0&&n>prec)n=prec;}else if(spec=='c'){buf[0]=va_arg(ap,int);n=1;}else if(spec=='d'||spec=='i'||spec=='u'||spec=='x'||spec=='X'||spec=='p'){unsigned long long v=len==2?va_arg(ap,unsigned long long):va_arg(ap,unsigned);if(spec=='d'||spec=='i'){if(len==2){if((long long)v<0){neg=1;v=-v;}}else if((int)v<0){neg=1;v=(unsigned)-(int)v;}}unsigned base=(spec=='x'||spec=='X'||spec=='p')?16:10;do{unsigned digit=__umoddi3(v,base);buf[n++]=(spec=='X'?"0123456789ABCDEF":"0123456789abcdef")[digit];v=__udivdi3(v,base);}while(v);if(prec>0){while(n<prec&&n<65)buf[n++]='0';}if(neg)buf[n++]='-';for(int i=0;i<n/2;i++){char c=buf[i];buf[i]=buf[n-1-i];buf[n-1-i]=c;}}else{puts("SIM_UNSUPPORTED_FORMAT");exit(3);}if(width>256)width=256;if(!left)for(int i=n;i<width;i++)emit(d,cap,&pos,zero?'0':' ');for(int i=0;i<n;i++)emit(d,cap,&pos,s[i]);if(left)for(int i=n;i<width;i++)emit(d,cap,&pos,' ');}if(d&&cap)d[pos<cap?pos:cap-1]=0;return pos;}
int snprintf(char*d,size_t n,const char*f,...){va_list a;va_start(a,f);int k=vsnprintf(d,n,f,a);va_end(a);return k;}int vfprintf(FILE*s,const char*f,va_list a){if(s!=stdout&&s!=stderr)return -1;return vsnprintf(0,0,f,a);}int fprintf(FILE*s,const char*f,...){va_list a;va_start(a,f);int k=vfprintf(s,f,a);va_end(a);return k;}int printf(const char*f,...){va_list a;va_start(a,f);int k=vfprintf(stdout,f,a);va_end(a);return k;}
int sscanf(const char*s,const char*f,...){va_list a;va_start(a,f);int count=0;while(*f){if(isspace(*f)){while(isspace(*s))s++;f++;continue;}if(*f!='%'){if(*f++!=*s++)break;continue;}f++;int width=0;while(isdigit(*f))width=width*10+*f++-'0';char c=*f++;if(c=='d'||c=='i'||c=='x'||c=='u'){char*e;long v=strtol(s,&e,c=='x'?16:c=='i'?0:10);if(e==s)break;*va_arg(a,int*)=v;s=e;count++;}else if(c=='s'){char*d=va_arg(a,char*);int k=0;while(isspace(*s))s++;if(!width)width=255;while(*s&&!isspace(*s)&&k<width)d[k++]=*s++;d[k]=0;if(!k)break;count++;}else break;}va_end(a);return count;}int fscanf(FILE*s,const char*f,...){return EOF;}
void qsort(void*p,size_t n,size_t z,int(*cmp)(const void*,const void*)){unsigned char*a=p;for(size_t i=1;i<n;i++)for(size_t j=i;j&&cmp(a+(j-1)*z,a+j*z)>0;j--)for(size_t k=0;k<z;k++){unsigned char c=a[(j-1)*z+k];a[(j-1)*z+k]=a[j*z+k];a[j*z+k]=c;}}
typedef union {unsigned long long v; struct {unsigned lo,hi;} w;} U64;
unsigned __umulsi3_highpart(unsigned a,unsigned b){unsigned a0=a&65535,a1=a>>16,b0=b&65535,b1=b>>16;unsigned p=a0*b0,t=a1*b0+(p>>16),carry=t<a1*b0;unsigned low=(t&65535)+a0*b1;return a1*b1+(t>>16)+(low>>16)+(carry<<16);}
unsigned long long __muldi3(unsigned long long a,unsigned long long b){U64 x={.v=a},y={.v=b},r;r.w.lo=x.w.lo*y.w.lo;r.w.hi=__umulsi3_highpart(x.w.lo,y.w.lo)+x.w.hi*y.w.lo+x.w.lo*y.w.hi;return r.v;}
unsigned long long __lshrdi3(unsigned long long a,int n){U64 x={.v=a},r;if(!n)return a;if(n>=64)return 0;if(n>=32){r.w.lo=x.w.hi>>(n-32);r.w.hi=0;}else{r.w.lo=(x.w.lo>>n)|(x.w.hi<<(32-n));r.w.hi=x.w.hi>>n;}return r.v;}
unsigned long long __ashldi3(unsigned long long a,int n){U64 x={.v=a},r;if(!n)return a;if(n>=64)return 0;if(n>=32){r.w.hi=x.w.lo<<(n-32);r.w.lo=0;}else{r.w.hi=(x.w.hi<<n)|(x.w.lo>>(32-n));r.w.lo=x.w.lo<<n;}return r.v;}

void private_heap_stats(unsigned *v){v[0]=heap_peak;v[1]=heap_live;v[2]=v[3]=v[4]=v[5]=0;for(Block*b=head;b;b=b->next){v[2]++;if(b->used){v[3]++;v[4]+=b->pad;}else v[5]+=b->size;} if(v[2]!=accounting_blocks||v[4]!=accounting_requested_live)abort();v[6]=accounting_peak_total;v[7]=accounting_peak_blocks*sizeof(Block);v[8]=head?accounting_min_free:(unsigned)(heap_end-heap_start);v[9]=accounting_requested_peak;}
