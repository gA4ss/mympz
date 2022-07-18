#include <mympz/mympz.h>

namespace mympz {

/**
  * @brief         两个基本的数值队列单元进行相加。
  * @param[in]     z        返回结果
  * @param[in]     x        大数结构
  * @param[in]     y        大数结构
  * @param[in]     n        多少位相加
  * @return        最后一组单位数相加后的进位。
  */
unit_t __add_units(number_t& z, const number_t &x, const number_t &y, size_t n) {
  unit_t c = 0, l = 0, t = 0;
  size_t i = 0;

  if (n == 0) return 0;

  //
  // 四个单位为一组进行运算
  //
#ifndef SMALL_FOOTPRINT
  while (n & ~3) {
    t = x[i];
    t = (t + c) & CALC_MASK;
    c = (t < c);
    l = (t + y[i]) & CALC_MASK;
    c += (l < t);
    z[i] = l;
    t = x[i+1];
    t = (t + c) & CALC_MASK;
    c = (t < c);
    l = (t + y[i+1]) & CALC_MASK;
    c += (l < t);
    z[i+1] = l;
    t = x[i+2];
    t = (t + c) & CALC_MASK;
    c = (t < c);
    l = (t + y[i+2]) & CALC_MASK;
    c += (l < t);
    z[i+2] = l;
    t = x[i+3];
    t = (t + c) & CALC_MASK;
    c = (t < c);
    l = (t + y[i+3]) & CALC_MASK;
    c += (l < t);
    z[i+3] = l;

    // 移动索引
    i += 4;
    n -= 4;
  }
# endif
  //
  // 计算随后的数据
  //
  while (n) {
    t = x[i];
    t = (t + c) & CALC_MASK;
    c = (t < c);
    l = (t + y[i]) & CALC_MASK;
    c += (l < t);
    z[i] = l;
    i++;
    n--;
  }
  return c;
}

/**
  * @brief         两个指定的单位长度相减
  * @param[in]     z        返回结果
  * @param[in]     x        大数结构
  * @param[in]     y        大数结构
  * @param[in]     n        多少位相减
  * @return        最后一组单位数相加后的进位。
  */
unit_t __sub_units(number_t& z, const number_t &x, const number_t &y, size_t n) {
  unit_t t1, t2;
  int c = 0;

  if (n == 0) return 0;

  size_t i = 0;
#ifndef SMALL_FOOTPRINT
  while (n & ~3) {
    t1 = x[i];
    t2 = y[i];
    z[i] = (t1 - t2 - c) & CALC_MASK;
    if (t1 != t2)
      c = (t1 < t2);
    t1 = x[i+1];
    t2 = y[i+1];
    z[i+1] = (t1 - t2 - c) & CALC_MASK;
    if (t1 != t2)
      c = (t1 < t2);
    t1 = x[i+2];
    t2 = y[i+2];
    z[i+2] = (t1 - t2 - c) & CALC_MASK;
    if (t1 != t2)
      c = (t1 < t2);
    t1 = x[i+3];
    t2 = y[i+3];
    z[i+3] = (t1 - t2 - c) & CALC_MASK;
    if (t1 != t2)
      c = (t1 < t2);
    i += 4;
    n -= 4;
  }
#endif
  while (n) {
    t1 = x[i];
    t2 = y[i];
    z[i] = (t1 - t2 - c) & CALC_MASK;
    if (t1 != t2)
      c = (t1 < t2);
    i++;
    n--;
  }
  return c;
}

} // namespace mympz