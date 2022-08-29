/**
  * @brief         Karatsuba算法
  * @param[out]    z是2乘以n2个元素的长度的队列
  * @param[in]     x是n2个长度的队列
  * @param[in]     y是n2个长度的队列
  * @param[in]     n2是2的次幂
  * @param[in]     t是2乘以n2个元素的长度的队列
  * @note 
  * x[0]*x[0]
 *  x[0]*x[0]+x[1]*x[1]+(x[0]-x[1])*(x[1]-x[0])
 *  x[1]*x[1]
  */
void __sqr_units_recursive(const number_ptr_t& y, const number_ptr_t& x, size_t n2,
                           const number_ptr_t& t)
{
  size_t n = n2 / 2;

  if (n2 == 4) {
    __sqr_4_units(y, x);
    return;
  } else if (n2 == 8) {
    __sqr_8_units(y, x);
    return;
  }

  if (n2 < CALC_SQR_RECURSIVE_SIZE_NORMAL) {
    __sqr_units_loop(y, x, n2, t);
    return;
  }

  /* y=(x[0]-x[1])*(x[1]-x[0]) */
  int c1 = __cmp(x, x+n, n);
  int zero = 0;
  if (c1 > 0)
    __sub_units(t, x, x+n, n);
  else if (c1 < 0)
    __sub_units(t, x+n, x, n);
  else
    zero = 1;

  /* 结果除非是0否则总是负数。 */
  number_ptr_t p = num_ptr_const_cast(t) + (n2 * 2);

  if (!zero) {
    __sqr_units_recursive(t + n2, t, n, p);
  } else {
    number_ptr_t tn2 = num_ptr_const_cast(t) + n2;
    for (size_t k = 0; k < n2; k++)
      tn2[k] = 0;
  }
  __sqr_units_recursive(y, x, n, p);
  __sqr_units_recursive(y + n2, x + n, n, p);

  /*-
    * t[32] 保存 (x[0]-x[1])*(x[1]-x[0]), 负数或0
    * y[10] 保存 (x[0]*y[0])
    * y[32] 保存 (y[1]*y[1])
    */

  c1 = static_cast<int>(__add_units(t, y, y+n2, n2));

  /* t[32] 是负数 */
  c1 -= static_cast<int>(__sub_units(t + n2, t, t + n2, n2));

  /*-
    * t[32] 保存 (x[0]-x[1])*(x[1]-x[0])+(x[0]*x[0])+(x[1]*x[1])
    * y[10] 保存 (x[0]*x[0])
    * y[32] 保存 (x[1]*x[1])
    * c1 保存 进位
    */
  c1 += static_cast<int>(__add_units(y + n, y + n, t + n2, n2));
  if (c1) {
    unit_t ln, lo;
    p = num_ptr_const_cast(y) + (n + n2);
    lo = *p;
    ln = (lo + c1) & CALC_MASK;
    *p = ln;

    /*
     * 溢出将在写入前停止
     */
    if (ln < static_cast<unit_t>(c1)) {
      do {
        p++;
        lo = *p;
        ln = (lo + 1) & CALC_MASK;
        *p = ln;
      } while (ln == 0);
    }
  }
}