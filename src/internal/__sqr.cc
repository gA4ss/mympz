#if defined(DISABLE_OPTIMIZE)
#define __sqr64(lo, ho, in)                         \
  {                                                 \
    unit_t l, h, m;                                 \
                                                    \
    h = (in);                                       \
    l = LBITS(h);                                   \
    h = HBITS(h);                                   \
    m = (l) * (h);                                  \
    l *= l;                                         \
    h *= h;                                         \
    h += (m & CALC_MASKh1) >> (UNIT_HALF_BITS - 1); \
    m = (m & CALC_MASKl) << (UNIT_HALF_BITS + 1);   \
    l = (l + m) & CALC_MASK;                        \
    if (l < m)                                      \
      h++;                                          \
    (lo) = l;                                       \
    (ho) = h;                                       \
  }

#define __sqr_unit __sqr64

/* __sqr_add_c_unit(a,i,c0,c1,c2)  -- c+=a[i]^2 三位数：c=(c2,c1,c0) */
#define __sqr_add_c_unit(a, i, c0, c1, c2) \
  do                                       \
  {                                        \
    unit_t lo, hi;                         \
    __sqr64(lo, hi, (a)[i]);               \
    c0 = (c0 + lo) & CALC_MASK;            \
    if (c0 < lo)                           \
      hi++;                                \
    c1 = (c1 + hi) & CALC_MASK;            \
    if (c1 < hi)                           \
      c2++;                                \
  } while (0)

/*
 * __sqr_add_c2_unit(a,i,c0,c1,c2) -- c+=2*a[i]*a[j]
 * 三位数：c=(c2,c1,c0)
 */
#define __sqr_add_c2_unit(a, i, j, c0, c1, c2) \
  __mul_add_c2_unit((a)[i], (a)[j], c0, c1, c2)
#endif

void __sqr_4_units(const number_ptr_t &y, const number_ptr_t &x)
{
  unit_t c1 = 0, c2 = 0, c3 = 0;

  __sqr_add_c_unit(x, 0, c1, c2, c3);
  y[0] = c1;
  c1 = 0;
  __sqr_add_c2_unit(x, 1, 0, c2, c3, c1);
  y[1] = c2;
  c2 = 0;
  __sqr_add_c_unit(x, 1, c3, c1, c2);
  __sqr_add_c2_unit(x, 2, 0, c3, c1, c2);
  y[2] = c3;
  c3 = 0;
  __sqr_add_c2_unit(x, 3, 0, c1, c2, c3);
  __sqr_add_c2_unit(x, 2, 1, c1, c2, c3);
  y[3] = c1;
  c1 = 0;
  __sqr_add_c_unit(x, 2, c2, c3, c1);
  __sqr_add_c2_unit(x, 3, 1, c2, c3, c1);
  y[4] = c2;
  c2 = 0;
  __sqr_add_c2_unit(x, 3, 2, c3, c1, c2);
  y[5] = c3;
  c3 = 0;
  __sqr_add_c_unit(x, 3, c1, c2, c3);
  y[6] = c1;
  y[7] = c2;
}

void __sqr_8_units(const number_ptr_t &y, const number_ptr_t &x)
{
  unit_t c1 = 0, c2 = 0, c3 = 0;

  __sqr_add_c_unit(x, 0, c1, c2, c3);
  y[0] = c1;
  c1 = 0;
  __sqr_add_c2_unit(x, 1, 0, c2, c3, c1);
  y[1] = c2;
  c2 = 0;
  __sqr_add_c_unit(x, 1, c3, c1, c2);
  __sqr_add_c2_unit(x, 2, 0, c3, c1, c2);
  y[2] = c3;
  c3 = 0;
  __sqr_add_c2_unit(x, 3, 0, c1, c2, c3);
  __sqr_add_c2_unit(x, 2, 1, c1, c2, c3);
  y[3] = c1;
  c1 = 0;
  __sqr_add_c_unit(x, 2, c2, c3, c1);
  __sqr_add_c2_unit(x, 3, 1, c2, c3, c1);
  __sqr_add_c2_unit(x, 4, 0, c2, c3, c1);
  y[4] = c2;
  c2 = 0;
  __sqr_add_c2_unit(x, 5, 0, c3, c1, c2);
  __sqr_add_c2_unit(x, 4, 1, c3, c1, c2);
  __sqr_add_c2_unit(x, 3, 2, c3, c1, c2);
  y[5] = c3;
  c3 = 0;
  __sqr_add_c_unit(x, 3, c1, c2, c3);
  __sqr_add_c2_unit(x, 4, 2, c1, c2, c3);
  __sqr_add_c2_unit(x, 5, 1, c1, c2, c3);
  __sqr_add_c2_unit(x, 6, 0, c1, c2, c3);
  y[6] = c1;
  c1 = 0;
  __sqr_add_c2_unit(x, 7, 0, c2, c3, c1);
  __sqr_add_c2_unit(x, 6, 1, c2, c3, c1);
  __sqr_add_c2_unit(x, 5, 2, c2, c3, c1);
  __sqr_add_c2_unit(x, 4, 3, c2, c3, c1);
  y[7] = c2;
  c2 = 0;
  __sqr_add_c_unit(x, 4, c3, c1, c2);
  __sqr_add_c2_unit(x, 5, 3, c3, c1, c2);
  __sqr_add_c2_unit(x, 6, 2, c3, c1, c2);
  __sqr_add_c2_unit(x, 7, 1, c3, c1, c2);
  y[8] = c3;
  c3 = 0;
  __sqr_add_c2_unit(x, 7, 2, c1, c2, c3);
  __sqr_add_c2_unit(x, 6, 3, c1, c2, c3);
  __sqr_add_c2_unit(x, 5, 4, c1, c2, c3);
  y[9] = c1;
  c1 = 0;
  __sqr_add_c_unit(x, 5, c2, c3, c1);
  __sqr_add_c2_unit(x, 6, 4, c2, c3, c1);
  __sqr_add_c2_unit(x, 7, 3, c2, c3, c1);
  y[10] = c2;
  c2 = 0;
  __sqr_add_c2_unit(x, 7, 4, c3, c1, c2);
  __sqr_add_c2_unit(x, 6, 5, c3, c1, c2);
  y[11] = c3;
  c3 = 0;
  __sqr_add_c_unit(x, 6, c1, c2, c3);
  __sqr_add_c2_unit(x, 7, 5, c1, c2, c3);
  y[12] = c1;
  c1 = 0;
  __sqr_add_c2_unit(x, 7, 6, c2, c3, c1);
  y[13] = c2;
  c2 = 0;
  __sqr_add_c_unit(x, 7, c3, c1, c2);
  y[14] = c3;
  y[15] = c1;
}

void __sqr_units(const number_ptr_t &y, const number_ptr_t &x, size_t n)
{
  if (n == 0)
    return;

  number_ptr_t _y = num_ptr_const_cast(y);
  number_ptr_t _x = num_ptr_const_cast(x);

#ifndef SMALL_FOOTPRINT
  while (n & ~3)
  {
    __sqr_unit(_y[0], _y[1], _x[0]);
    __sqr_unit(_y[2], _y[3], _x[1]);
    __sqr_unit(_y[4], _y[5], _x[2]);
    __sqr_unit(_y[6], _y[7], _x[3]);
    _x += 4;
    _y += 8;
    n -= 4;
  }
#endif
  while (n)
  {
    __sqr_unit(_y[0], _y[1], _x[0]);
    _x++;
    _y += 2;
    n--;
  }
}

/* t的长度必须是2*n个字 */
void __sqr_units_loop(const number_ptr_t &y, const number_ptr_t &x, size_t nx,
                      const number_ptr_t &t)
{
  size_t max = nx * 2;
  number_ptr_t _y = num_ptr_const_cast(y);
  number_ptr_t _x = num_ptr_const_cast(x);

  _y[0] = _y[max - 1] = 0;
  _y++;

  size_t j = nx;
  if (--j > 0)
  {
    _x++;
    _y[j] = __mul_units_unit(_y, _x, j, _x[-1]);
    _y += 2;
  }

  for (int i = static_cast<int>(nx) - 2; i > 0; i--)
  {
    j--;
    _x++;
    _y[j] = __mul_add_units(_y, _x, j, _x[-1]);
    _y += 2;
  }

  __add_units(y, y, y, max);

  /* 这里不会产生进位 */

  __sqr_units(t, x, nx);
  __add_units(y, y, t, max);
}

//
// 递归实现
//
#if defined(CALC_RECURSION)
#include "__sqr_recursive.cc"
#endif