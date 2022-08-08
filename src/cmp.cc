#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         带符号比较两个大数的大小。
  * @param[in]     x      大数结构
  * @param[in]     y      大数结构
  * @note          如果x或者y中存在null值，则谁是null值，谁大。
  * @return        一个有符号整型表明比较结果。
  * @retval        -1    x < y
  * @retval        0     x = y
  * @retval        1     x > y
  */
int cmp(const bignum_t& x, const bignum_t& y) {
  ///< 谁是null值，谁大。
  if (is_null(x) || is_null(y)) {
    if (!is_null(x))
      return -1;
    else if (!is_null(y))
      return 1;
    else
      return 0;
  }

  if (x.neg != y.neg) {
    if (x.neg)
      return -1;
    else
      return 1;
  }

  int gt, lt;
  if (x.neg == 0) {
    gt = 1;
    lt = -1;
  } else {
    gt = -1;
    lt = 1;
  }

  if (bn_size(x) > bn_size(y))
    return gt;
  if (bn_size(x) < bn_size(y))
    return lt;

  unit_t t1, t2;
  for (int i = static_cast<int>(bn_size(x) - 1); i >= 0; i--) {
    t1 = x.number[i];
    t2 = y.number[i];
    if (t1 > t2)
      return gt;
    if (t1 < t2)
      return lt;
  }
  return 0;
}

/**
  * @brief         无符号比较两个大数的大小。
  * @param[in]     x      大数结构
  * @param[in]     y      大数结构
  * @note          仅比较数值大小。
  * @return        一个有符号整型表明比较结果。
  * @retval        -1    x < y
  * @retval        0     x = y
  * @retval        1     x > y
  */
int ucmp(const bignum_t& x, const bignum_t& y) {
  unit_t t1, t2;

  ///< 这里可能存在一些问题。
  int i = static_cast<int>(bn_size(x) - bn_size(y));
  if (i != 0)
    return i;

  for (i = static_cast<int>(bn_size(x) - 1); i >= 0; i--) {
    t1 = x.number[i];
    t2 = y.number[i];
    if (t1 != t2)
      return ((t1 > t2) ? 1 : -1);
  }
  return 0;
}

} // namespace mympz