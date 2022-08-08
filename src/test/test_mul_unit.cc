#include <iostream>
#include <ctype.h>

#define UNIT_ULONG       unsigned long
#define LBITS(a)         ((a)&0xffffffffL)
#define HBITS(a)         (((a)>>32)&0xffffffffL)
#define L2HBITS(a)       (((a)<<32)&0xffffffffffffffffL)

#define mul64(l,h,bl,bh) { \
  UNIT_ULONG m,m1,lt,ht; \
  lt=l; \
  ht=h; \
  m =(bh)*(lt); \
  lt=(bl)*(lt); \
  m1=(bl)*(ht); \
  ht =(bh)*(ht); \
  m=(m+m1)&0xffffffffffffffffL; if (m < m1) ht+=L2HBITS((UNIT_ULONG)1); \
  ht+=HBITS(m); \
  m1=L2HBITS(m); \
  lt=(lt+m1)&0xffffffffffffffffL; if (lt < m1) ht++; \
  (l)=lt; \
  (h)=ht; \
}

int main(int argc, char* argv[]) {
  UNIT_ULONG x = 0xFFAACCDD11223344,y = 0xBBCCAADD55667788;
  UNIT_ULONG lo = LBITS(x), hi = HBITS(x);    \
  UNIT_ULONG bl = LBITS(y), bh = HBITS(y);    \
  mul64(lo,hi,bl,bh);
  std::cout << "lo = " << std::hex << lo << " " << "hi = " << hi << std::endl;
  return 0;
}