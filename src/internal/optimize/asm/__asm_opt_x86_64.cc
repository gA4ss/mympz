unit_t __opt_add_units(unit_t *rp, const unit_t *ap, const unit_t *bp, size_t n)
{
  unit_t ret;
  size_t i = 0;

  if (n == 0)
    return 0;

  asm volatile("       subq    %0,%0           \n" /* clear carry */
               "       jmp     1f              \n"
               ".p2align 4                     \n"
               "1:     movq    (%4,%2,8),%0    \n"
               "       adcq    (%5,%2,8),%0    \n"
               "       movq    %0,(%3,%2,8)    \n"
               "       lea     1(%2),%2        \n"
               "       dec     %1              \n"
               "       jnz     1b              \n"
               "       sbbq    %0,%0           \n"
               : "=&r"(ret), "+c"(n), "+r"(i)
               : "r"(rp), "r"(ap), "r"(bp)
               : "cc", "memory");

  return ret & 1;
}

unit_t __opt_sub_units(unit_t *rp, const unit_t *ap, const unit_t *bp, size_t n)
{
  unit_t ret;
  size_t i = 0;

  if (n == 0)
    return 0;

  asm volatile("       subq    %0,%0           \n" /* clear borrow */
               "       jmp     1f              \n"
               ".p2align 4                     \n"
               "1:     movq    (%4,%2,8),%0    \n"
               "       sbbq    (%5,%2,8),%0    \n"
               "       movq    %0,(%3,%2,8)    \n"
               "       lea     1(%2),%2        \n"
               "       dec     %1              \n"
               "       jnz     1b              \n"
               "       sbbq    %0,%0           \n"
               : "=&r"(ret), "+c"(n), "+r"(i)
               : "r"(rp), "r"(ap), "r"(bp)
               : "cc", "memory");

  return ret & 1;
}

// unit_t __opt_div_units(unit_t h, unit_t l, unit_t d)
// {
//   unit_t ret, waste;

//   asm("divq      %4"
//       : "=a"(ret), "=d"(waste)
//       : "a"(l), "d"(h), "r"(d)
//       : "cc");

//   return ret;
// }

#if defined(CPU_X86)
#define __div_unit(n0, n1, d0)      \
  ({                                \
    asm volatile(                   \
        register UNIT_ULONG q;      \
        "divl   %4"                 \
        : "=a"(q), "=d"(rem)        \
        : "a"(n1), "d"(n0), "r"(d0) \
        : "cc");                    \
    q;                              \
  })
#define REMAINDER_IS_ALREADY_CALCULATED
#endif

#define __div_unit(n0, n1, d0)      \
  ({                                \
    register UNIT_ULONG q;          \
    asm volatile(                   \
        "divq   %4"                 \
        : "=a"(q), "=d"(rem)        \
        : "a"(n1), "d"(n0), "r"(d0) \
        : "cc");                    \
    q;                              \
  })
#define REMAINDER_IS_ALREADY_CALCULATED

#define __mul_unit(r, a, word, carry) \
  do                                  \
  {                                   \
    register UNIT_ULONG high, low;    \
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

/*-
 * "m"(a), "+m"(r)      DirectPath p-code的有利方法，内敛技巧；
 * "g"(0)               让编译器来决定它想把0的值保存在哪里。
 */
#define __mul_add_unit(r, a, word, carry) \
  do                                      \
  {                                       \
    register UNIT_ULONG high, low;        \
    asm("mulq %3"                         \
        : "=a"(low), "=d"(high)           \
        : "a"(word), "m"(a)               \
        : "cc");                          \
    asm("addq %2,%0; adcq %3,%1"          \
        : "+r"(carry), "+d"(high)         \
        : "a"(low), "g"(0)                \
        : "cc");                          \
    asm("addq %2,%0; adcq %3,%1"          \
        : "+m"(r), "+d"(high)             \
        : "r"(carry), "g"(0)              \
        : "cc");                          \
    carry = high;                         \
  } while (0)

/* __mul_add_c_unit(a,b,c0,c1,c2)  -- c+=a*b for three word number c=(c2,c1,c0) */
#define __mul_add_c_unit(a, b, c0, c1, c2)   \
  do                                         \
  {                                          \
    UNIT_ULONG t1, t2;                       \
    asm("mulq %3"                            \
        : "=a"(t1), "=d"(t2)                 \
        : "a"(a), "m"(b)                     \
        : "cc");                             \
    asm("addq %3,%0; adcq %4,%1; adcq %5,%2" \
        : "+r"(c0), "+r"(c1), "+r"(c2)       \
        : "r"(t1), "r"(t2), "g"(0)           \
        : "cc");                             \
  } while (0)

/* __mul_add_c2_unit(a,b,c0,c1,c2) -- c+=2*a*b for three word number c=(c2,c1,c0) */
#define __mul_add_c2_unit(a, b, c0, c1, c2)  \
  do                                         \
  {                                          \
    UNIT_ULONG t1, t2;                       \
    asm("mulq %3"                            \
        : "=a"(t1), "=d"(t2)                 \
        : "a"(a), "m"(b)                     \
        : "cc");                             \
    asm("addq %3,%0; adcq %4,%1; adcq %5,%2" \
        : "+r"(c0), "+r"(c1), "+r"(c2)       \
        : "r"(t1), "r"(t2), "g"(0)           \
        : "cc");                             \
    asm("addq %3,%0; adcq %4,%1; adcq %5,%2" \
        : "+r"(c0), "+r"(c1), "+r"(c2)       \
        : "r"(t1), "r"(t2), "g"(0)           \
        : "cc");                             \
  } while (0)

#define __sqr_unit(r0, r1, a) \
  asm("mulq %2"               \
      : "=a"(r0), "=d"(r1)    \
      : "a"(a)                \
      : "cc");

/* __sqr_add_c_unit(a,i,c0,c1,c2)  -- c+=a[i]^2 for three word number c=(c2,c1,c0) */
#define __sqr_add_c_unit(a, i, c0, c1, c2)   \
  do                                         \
  {                                          \
    UNIT_ULONG t1, t2;                       \
    asm("mulq %2"                            \
        : "=a"(t1), "=d"(t2)                 \
        : "a"(a[i])                          \
        : "cc");                             \
    asm("addq %3,%0; adcq %4,%1; adcq %5,%2" \
        : "+r"(c0), "+r"(c1), "+r"(c2)       \
        : "r"(t1), "r"(t2), "g"(0)           \
        : "cc");                             \
  } while (0)

/* __sqr_add_c2_unit(a,i,c0,c1,c2) -- c+=2*a[i]*a[j] for three word number c=(c2,c1,c0) */
#define __sqr_add_c2_unit(a, i, j, c0, c1, c2) \
  __mul_add_c2_unit((a)[i], (a)[j], c0, c1, c2)
