unit_t __opt_add_units(unit_t *rp, const unit_t *ap, const unit_t *bp, size_t n) {
  unit_t ret;
  size_t i = 0;

  if (n == 0)
    return 0;

  asm volatile ("       subq    %0,%0           \n" /* clear carry */
                "       jmp     1f              \n"
                ".p2align 4                     \n"
                "1:     movq    (%4,%2,8),%0    \n"
                "       adcq    (%5,%2,8),%0    \n"
                "       movq    %0,(%3,%2,8)    \n"
                "       lea     1(%2),%2        \n"
                "       dec     %1              \n"
                "       jnz     1b              \n"
                "       sbbq    %0,%0           \n"
                :"=&r" (ret), "+c"(n), "+r"(i)
                :"r"(rp), "r"(ap), "r"(bp)
                :"cc", "memory");

  return ret & 1;
}

unit_t __opt_sub_units(unit_t *rp, const unit_t *ap, const unit_t *bp, size_t n) {
  unit_t ret;
  size_t i = 0;

  if (n == 0)
    return 0;

  asm volatile ("       subq    %0,%0           \n" /* clear borrow */
                "       jmp     1f              \n"
                ".p2align 4                     \n"
                "1:     movq    (%4,%2,8),%0    \n"
                "       sbbq    (%5,%2,8),%0    \n"
                "       movq    %0,(%3,%2,8)    \n"
                "       lea     1(%2),%2        \n"
                "       dec     %1              \n"
                "       jnz     1b              \n"
                "       sbbq    %0,%0           \n"
                :"=&r" (ret), "+c"(n), "+r"(i)
                :"r"(rp), "r"(ap), "r"(bp)
                :"cc", "memory");

  return ret & 1;
}

#define __mul_unit(r, a, word, carry) \
  do                                  \
  {                                   \
    register uint64_t high, low;      \
    asm("mulq %3"                     \
        : "=a"(low), "=d"(high)       \
        : "a"(word), "g"(a)           \
        : "cc");                      \
    asm("addq %2,%0; adcq %3,%1"      \
        : "+r"(carry), "+d"(high)     \
        : "a"(low), "g"(0)            \
        : "cc");                      \
    (r) = carry, carry = high;        \
  } while (0)
