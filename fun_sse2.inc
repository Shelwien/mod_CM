
//typedef unsigned int uint;

#define ASM __asm__

extern "C" {

//__declspec(noinline)
int dot_product( short *t, short *w, int n ) {
  int sum;
  ASM ("\
  add $7,%%ecx; \
  and $-8,%%ecx; \
  jz 1f; \
  sub $16,%%eax; \
  sub $16,%%edx; \
  pxor %%xmm0,%%xmm0; \
2:   movdqa (%%eax,%%ecx,2),%%xmm1; \
  pmaddwd (%%edx,%%ecx,2),%%xmm1; \
  psrad $8,%%xmm1; \
  paddd %%xmm1,%%xmm0; \
  sub $8,%%ecx; \
  ja 2b; \
  movhlps %%xmm0,%%xmm1; \
  paddd %%xmm1,%%xmm0; \
  pshuflw $0xE,%%xmm0,%%xmm1; \
  paddd %%xmm1,%%xmm0; \
  movd %%xmm0,%%eax; \
1:   " : "=a"(sum) : "a"(t),"d"(w),"c"(n) : "xmm0","xmm1"
  );

  return sum;
}

//__declspec(noinline)
void train( short *t, short *w, int n, int err ) {
  ASM ("\
  add $15,%%ecx; \
  and $-16,%%ecx; \
  jz 1f; \
  sub $32,%%eax; \
  sub $32,%%edx; \
  movd %3,%%xmm0; \
  pcmpeqb %%xmm6,%%xmm6; \
  pshuflw $0,%%xmm0,%%xmm0; \
  psrlw $15,%%xmm6; \
  punpcklqdq %%xmm0,%%xmm0; \
2:   movdqa 0(%%eax,%%ecx,2),%%xmm3; \
  movdqa 16(%%eax,%%ecx,2),%%xmm5; \
  paddsw %%xmm3,%%xmm3; \
  paddsw %%xmm5,%%xmm5; \
  pmulhw %%xmm0,%%xmm3; \
  pmulhw %%xmm0,%%xmm5; \
  paddsw %%xmm6,%%xmm3; \
  paddsw %%xmm6,%%xmm5; \
  psraw $1,%%xmm3; \
  psraw $1,%%xmm5; \
  paddsw 0(%%edx,%%ecx,2),%%xmm3; \
  paddsw 16(%%edx,%%ecx,2),%%xmm5; \
  movdqa %%xmm3,0(%%edx,%%ecx,2); \
  movdqa %%xmm5,16(%%edx,%%ecx,2); \
  sub $16,%%ecx; \
  ja 2b; \
1:   " :  : "a"(t),"d"(w),"c"(n),"rm"(err) : "xmm0","xmm3","xmm5","xmm6"
  );

}

}

