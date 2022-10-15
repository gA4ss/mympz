#if defined(DISABLE_OPTIMIZE)
//
// 交叉相乘
// 结果的低位保存在"l"，高位保存在"h"。
//
#define __mul64(l, h, bl, bh)       \
  {                                 \
    UNIT_ULONG m, m1, lt, ht;       \
    lt = l;                         \
    ht = h;                         \
    m = (bh) * (lt);                \
    lt = (bl) * (lt);               \
    m1 = (bl) * (ht);               \
    ht = (bh) * (ht);               \
    m = (m + m1) & CALC_MASK;       \
    if (m < m1)                     \
      ht += L2HBITS((UNIT_ULONG)1); \
    ht += HBITS(m);                 \
    m1 = L2HBITS(m);                \
    lt = (lt + m1) & CALC_MASK;     \
    if (lt < m1)                    \
      ht++;                         \
    (l) = lt;                       \
    (h) = ht;                       \
  }

//
// a 乘以 b(bl,bh)加上进位，最后再加上
// r的结果，高位放到c上，低位放到r上。
//
#define __mul_add_unit(r, a, b, c) \
  {                                \
    UNIT_ULONG l, h, bl, bh;       \
    bl = LBITS(b);                 \
    bh = HBITS(b);                 \
                                   \
    h = (a);                       \
    l = LBITS(h);                  \
    h = HBITS(h);                  \
    __mul64(l, h, (bl), (bh));     \
                                   \
    l = (l + (c)) & CALC_MASK;     \
    if (l < (c))                   \
      h++;                         \
    (c) = (r);                     \
    l = (l + (c)) & CALC_MASK;     \
    if (l < (c))                   \
      h++;                         \
    (c) = h & CALC_MASK;           \
    (r) = l;                       \
  }

//
// c是进位，与mul64不同的是最终的计算
// 结果会加上c的进位数值。
//
#define __mul_unit(r, a, b, c) \
  {                            \
    UNIT_ULONG l, h, bl, bh;   \
    bl = LBITS(b);             \
    bh = HBITS(b);             \
                               \
    h = (a);                   \
    l = LBITS(h);              \
    h = HBITS(h);              \
    __mul64(l, h, (bl), (bh)); \
                               \
    l += (c);                  \
    if ((l & CALC_MASK) < (c)) \
      h++;                     \
    (c) = h & CALC_MASK;       \
    (r) = l & CALC_MASK;       \
  }

//
// __mul_add_c_unit(a,b,c0,c1,c2)  -- c+=a*b 三位数：c=(c2,c1,c0)
// 对宏变量hi的增加不会产生溢出，因为乘法结果的高位不可
// 能全是1。
//
#define __mul_add_c_unit(a, b, c0, c1, c2)   \
  do                                         \
  {                                          \
    UNIT_ULONG lo = LBITS(a), hi = HBITS(a); \
    UNIT_ULONG bl = LBITS(b), bh = HBITS(b); \
    __mul64(lo, hi, bl, bh);                 \
    c0 = (c0 + lo) & CALC_MASK;              \
    if (c0 < lo)                             \
      hi++;                                  \
    c1 = (c1 + hi) & CALC_MASK;              \
    if (c1 < hi)                             \
      c2++;                                  \
  } while (0)

//
// __mul_add_c2_unit(a,b,c0,c1,c2) -- c+=2*a*b 三位数：c=(c2,c1,c0)
//
#define __mul_add_c2_unit(a, b, c0, c1, c2)  \
  do                                         \
  {                                          \
    UNIT_ULONG tt;                           \
    UNIT_ULONG lo = LBITS(a), hi = HBITS(a); \
    UNIT_ULONG bl = LBITS(b), bh = HBITS(b); \
    __mul64(lo, hi, bl, bh);                 \
    tt = hi;                                 \
    c0 = (c0 + lo) & CALC_MASK;              \
    if (c0 < lo)                             \
      tt++;                                  \
    c1 = (c1 + tt) & CALC_MASK;              \
    if (c1 < tt)                             \
      c2++;                                  \
    c0 = (c0 + lo) & CALC_MASK;              \
    if (c0 < lo)                             \
      hi++;                                  \
    c1 = (c1 + hi) & CALC_MASK;              \
    if (c1 < hi)                             \
      c2++;                                  \
  } while (0)

#endif

void __mul_4_units(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y)
{
  unit_t c1 = 0, c2 = 0, c3 = 0;

  __mul_add_c_unit(x[0], y[0], c1, c2, c3);
  z[0] = c1;
  c1 = 0;
  __mul_add_c_unit(x[0], y[1], c2, c3, c1);
  __mul_add_c_unit(x[1], y[0], c2, c3, c1);
  z[1] = c2;
  c2 = 0;
  __mul_add_c_unit(x[2], y[0], c3, c1, c2);
  __mul_add_c_unit(x[1], y[1], c3, c1, c2);
  __mul_add_c_unit(x[0], y[2], c3, c1, c2);
  z[2] = c3;
  c3 = 0;
  __mul_add_c_unit(x[0], y[3], c1, c2, c3);
  __mul_add_c_unit(x[1], y[2], c1, c2, c3);
  __mul_add_c_unit(x[2], y[1], c1, c2, c3);
  __mul_add_c_unit(x[3], y[0], c1, c2, c3);
  z[3] = c1;
  c1 = 0;
  __mul_add_c_unit(x[3], y[1], c2, c3, c1);
  __mul_add_c_unit(x[2], y[2], c2, c3, c1);
  __mul_add_c_unit(x[1], y[3], c2, c3, c1);
  z[4] = c2;
  c2 = 0;
  __mul_add_c_unit(x[2], y[3], c3, c1, c2);
  __mul_add_c_unit(x[3], y[2], c3, c1, c2);
  z[5] = c3;
  c3 = 0;
  __mul_add_c_unit(x[3], y[3], c1, c2, c3);
  z[6] = c1;
  z[7] = c2;
}

void __mul_8_units(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y)
{
  unit_t c1 = 0, c2 = 0, c3 = 0;

  __mul_add_c_unit(x[0], y[0], c1, c2, c3);
  z[0] = c1;
  c1 = 0;
  __mul_add_c_unit(x[0], y[1], c2, c3, c1);
  __mul_add_c_unit(x[1], y[0], c2, c3, c1);
  z[1] = c2;
  c2 = 0;
  __mul_add_c_unit(x[2], y[0], c3, c1, c2);
  __mul_add_c_unit(x[1], y[1], c3, c1, c2);
  __mul_add_c_unit(x[0], y[2], c3, c1, c2);
  z[2] = c3;
  c3 = 0;
  __mul_add_c_unit(x[0], y[3], c1, c2, c3);
  __mul_add_c_unit(x[1], y[2], c1, c2, c3);
  __mul_add_c_unit(x[2], y[1], c1, c2, c3);
  __mul_add_c_unit(x[3], y[0], c1, c2, c3);
  z[3] = c1;
  c1 = 0;
  __mul_add_c_unit(x[4], y[0], c2, c3, c1);
  __mul_add_c_unit(x[3], y[1], c2, c3, c1);
  __mul_add_c_unit(x[2], y[2], c2, c3, c1);
  __mul_add_c_unit(x[1], y[3], c2, c3, c1);
  __mul_add_c_unit(x[0], y[4], c2, c3, c1);
  z[4] = c2;
  c2 = 0;
  __mul_add_c_unit(x[0], y[5], c3, c1, c2);
  __mul_add_c_unit(x[1], y[4], c3, c1, c2);
  __mul_add_c_unit(x[2], y[3], c3, c1, c2);
  __mul_add_c_unit(x[3], y[2], c3, c1, c2);
  __mul_add_c_unit(x[4], y[1], c3, c1, c2);
  __mul_add_c_unit(x[5], y[0], c3, c1, c2);
  z[5] = c3;
  c3 = 0;
  __mul_add_c_unit(x[6], y[0], c1, c2, c3);
  __mul_add_c_unit(x[5], y[1], c1, c2, c3);
  __mul_add_c_unit(x[4], y[2], c1, c2, c3);
  __mul_add_c_unit(x[3], y[3], c1, c2, c3);
  __mul_add_c_unit(x[2], y[4], c1, c2, c3);
  __mul_add_c_unit(x[1], y[5], c1, c2, c3);
  __mul_add_c_unit(x[0], y[6], c1, c2, c3);
  z[6] = c1;
  c1 = 0;
  __mul_add_c_unit(x[0], y[7], c2, c3, c1);
  __mul_add_c_unit(x[1], y[6], c2, c3, c1);
  __mul_add_c_unit(x[2], y[5], c2, c3, c1);
  __mul_add_c_unit(x[3], y[4], c2, c3, c1);
  __mul_add_c_unit(x[4], y[3], c2, c3, c1);
  __mul_add_c_unit(x[5], y[2], c2, c3, c1);
  __mul_add_c_unit(x[6], y[1], c2, c3, c1);
  __mul_add_c_unit(x[7], y[0], c2, c3, c1);
  z[7] = c2;
  c2 = 0;
  __mul_add_c_unit(x[7], y[1], c3, c1, c2);
  __mul_add_c_unit(x[6], y[2], c3, c1, c2);
  __mul_add_c_unit(x[5], y[3], c3, c1, c2);
  __mul_add_c_unit(x[4], y[4], c3, c1, c2);
  __mul_add_c_unit(x[3], y[5], c3, c1, c2);
  __mul_add_c_unit(x[2], y[6], c3, c1, c2);
  __mul_add_c_unit(x[1], y[7], c3, c1, c2);
  z[8] = c3;
  c3 = 0;
  __mul_add_c_unit(x[2], y[7], c1, c2, c3);
  __mul_add_c_unit(x[3], y[6], c1, c2, c3);
  __mul_add_c_unit(x[4], y[5], c1, c2, c3);
  __mul_add_c_unit(x[5], y[4], c1, c2, c3);
  __mul_add_c_unit(x[6], y[3], c1, c2, c3);
  __mul_add_c_unit(x[7], y[2], c1, c2, c3);
  z[9] = c1;
  c1 = 0;
  __mul_add_c_unit(x[7], y[3], c2, c3, c1);
  __mul_add_c_unit(x[6], y[4], c2, c3, c1);
  __mul_add_c_unit(x[5], y[5], c2, c3, c1);
  __mul_add_c_unit(x[4], y[6], c2, c3, c1);
  __mul_add_c_unit(x[3], y[7], c2, c3, c1);
  z[10] = c2;
  c2 = 0;
  __mul_add_c_unit(x[4], y[7], c3, c1, c2);
  __mul_add_c_unit(x[5], y[6], c3, c1, c2);
  __mul_add_c_unit(x[6], y[5], c3, c1, c2);
  __mul_add_c_unit(x[7], y[4], c3, c1, c2);
  z[11] = c3;
  c3 = 0;
  __mul_add_c_unit(x[7], y[5], c1, c2, c3);
  __mul_add_c_unit(x[6], y[6], c1, c2, c3);
  __mul_add_c_unit(x[5], y[7], c1, c2, c3);
  z[12] = c1;
  c1 = 0;
  __mul_add_c_unit(x[6], y[7], c2, c3, c1);
  __mul_add_c_unit(x[7], y[6], c2, c3, c1);
  z[13] = c2;
  c2 = 0;
  __mul_add_c_unit(x[7], y[7], c3, c1, c2);
  z[14] = c3;
  z[15] = c1;
}

unit_t __mul_units_unit(const number_ptr_t &z, const number_ptr_t &x, size_t nx, unit_t y)
{
  if (nx == 0)
    return 0;

  unit_t carry = 0;
  size_t i = 0;

#ifndef SMALL_FOOTPRINT
  while (nx & ~3)
  {
    __mul_unit(z[i], x[i], y, carry);
    __mul_unit(z[i + 1], x[i + 1], y, carry);
    __mul_unit(z[i + 2], x[i + 2], y, carry);
    __mul_unit(z[i + 3], x[i + 3], y, carry);
    i += 4;
    nx -= 4;
  }
#endif
  while (nx)
  {
    __mul_unit(z[i], x[i], y, carry);
    i++;
    nx--;
  }
  return carry;
}

unit_t __mul_add_units(const number_ptr_t &z, const number_ptr_t &x, size_t nx,
                       const unit_t &y)
{
  unit_t c = 0;
  my_assert(nx >= 0, "size of numbert x = \'%ul\' less than 0.", nx);
  if (nx == 0)
    return 0;

  size_t i = 0;

#ifndef SMALL_FOOTPRINT
  while (nx & ~3)
  {
    __mul_add_unit(z[i], x[i], y, c);
    __mul_add_unit(z[i + 1], x[i + 1], y, c);
    __mul_add_unit(z[i + 2], x[i + 2], y, c);
    __mul_add_unit(z[i + 3], x[i + 3], y, c);
    i += 4;
    nx -= 4;
  }
#endif

  while (nx)
  {
    __mul_add_unit(z[i], x[i], y, c);
    i++;
    nx--;
  }
  return c;
}

void __mul_units_loop(const number_ptr_t &z, const number_ptr_t &x, size_t nx,
                      const number_ptr_t &y, size_t ny)
{
  //
  // 如果x的长度小于y,则交换两个大数队列。
  //
  number_ptr_t _x;
  number_ptr_t _y;
  if (nx < ny)
  {
    size_t t = nx;
    nx = ny;
    ny = t;

    _x = num_ptr_const_cast(y);
    _y = num_ptr_const_cast(x);
  }
  else
  {
    _x = num_ptr_const_cast(x);
    _y = num_ptr_const_cast(y);
  }

  number_ptr_t _z = num_ptr_const_cast(z);
  // if (ny == 0) {
  //   _z = _x;   // 将x赋给z
  //   return;
  // }
  if (ny == 0)
    return;

  //
  // 进位保存在nx的索引，其余的保存在z的[0, nx-1]范围中。
  //
  size_t j = 0;
  number_ptr_t _zz = _z + nx;
  _zz[0] = __mul_units_unit(_z, _x, nx, _y[j]);
  for (;;)
  {
    if (--ny == 0)
      return;
    _zz[1] = __mul_add_units(_z + 1, _x, nx, _y[j + 1]);
    if (--ny == 0)
      return;
    _zz[2] = __mul_add_units(_z + 2, _x, nx, _y[j + 2]);
    if (--ny == 0)
      return;
    _zz[3] = __mul_add_units(_z + 3, _x, nx, _y[j + 3]);
    if (--ny == 0)
      return;
    _zz[4] = __mul_add_units(_z + 4, _x, nx, _y[j + 4]);

    j += 4;
    _z += 4;
    _zz += 4;
  }
}

void __mul_units_low_loop(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n)
{
  __mul_units_unit(z, x, n, y[0]);

  number_ptr_t _z = num_ptr_const_cast(z);
  number_ptr_t _y = num_ptr_const_cast(y);
  for (;;)
  {
    if (--n == 0)
      return;
    __mul_add_units(_z + 1, x, n, _y[1]);
    if (--n == 0)
      return;
    __mul_add_units(_z + 2, x, n, _y[2]);
    if (--n == 0)
      return;
    __mul_add_units(_z + 3, x, n, _y[3]);
    if (--n == 0)
      return;
    __mul_add_units(_z + 4, x, n, _y[4]);
    _z += 4;
    _y += 4;
  }
}

//
// 递归实现
//
#if defined(CALC_RECURSION)
#include "__mul_recursive.cc"
#endif

number_t __mul_units(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl)
{
  number_t z;

  int i = static_cast<int>(xl) - static_cast<int>(yl);
  if ((xl >= CALC_MULL_SIZE_NORMAL) && (yl >= CALC_MULL_SIZE_NORMAL))
  {
    //
    // 1.乘数与被乘数大于某个长度
    // 2.乘数与被乘数的长度仅差1位
    // 才使用分治算法
    //
    if (i >= -1 && i <= 1)
    { // -1 <= i <= 1
      number_t t;
      //
      // 求最小二的幂，或等于其中最长的两个数字。
      //
      int j = 0; ///< 保存最大长度的最小二次幂
      if (i >= 0)
      { // xl >= yl
        j = static_cast<int>(__count_bits(static_cast<unit_t>(xl)));
      }
      else if (i == -1)
      { // xl < yl
        j = static_cast<int>(__count_bits(static_cast<unit_t>(yl)));
      }

      //
      // j,k这个组合相当于取了2的取整数，
      // 随后使用k的长度来保存乘法结果。
      //
      j = 1 << (j - 1);
      my_assert(j <= static_cast<int>(xl) || j <= static_cast<int>(yl),
                "j > xl || j > yl,j = %l.", j);
      int k = j + j;

      if (static_cast<int>(xl) > j || static_cast<int>(yl) > j)
      {
        num_resize(t, k * 4);
        num_resize(z, k * 4);
        __mul_part_units_recursive(num_ptr(z), x, y,
                                   j, xl - j, yl - j, num_ptr(t));
      }
      else
      { // xl <= j || yl <= j
        num_resize(t, k * 2);
        num_resize(z, k * 2);
        __mul_units_recursive(num_ptr(z), x, y,
                              j, xl - j, yl - j, num_ptr(t));
      }
    }
    else
    {
      num_resize(z, xl + yl + 1);
      __mul_units_loop(num_ptr(z), x, xl, y, yl);
    }
  }
  else
  {
    //
    // 小于CALC_MULL_SIZE_NORMAL位数则直接使用循环来处理
    //
    num_resize(z, xl + yl + 1);
    __mul_units_loop(num_ptr(z), x, xl, y, yl);
  }
  __clear_number_head_zero(z);
  return z;
}