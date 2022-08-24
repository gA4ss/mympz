/**
  * @brief         比较两个等长度的大数队列大小
  * @param[in]     x      大数结构
  * @param[in]     y      大数结构
  * @param[in]     n      比较长度
  * 
  * @return        一个有符号整型表明比较结果。
  * @retval        -1    x < y
  * @retval        0     x = y
  * @retval        1     x > y
  */
int __cmp(const number_ptr_t& x, const number_ptr_t& y, int n) {
  unit_t xx, yy;

  if (n == 0)
    return 0;

  xx = x[n - 1];
  yy = y[n - 1];
  if (xx != yy)
    return ((xx > yy) ? 1 : -1);
  
  for (int i = n - 2; i >= 0; i--) {
    xx = x[i];
    yy = y[i];
    if (xx != yy)
      return ((xx > yy) ? 1 : -1);
  }
  return 0;
}

/**
  * @brief         比较两个大数队列大小
  * @param[in]     x      大数结构
  * @param[in]     y      大数结构
  * @param[in]     cl     公共长度等于，min(len(x)，len(y)))。
  * @param[in]     dl     两个长度之间的差量等于，len(x)-len(y)，可能为负数。
  * 
  * @return        一个有符号整型表明比较结果。
  * @retval        -1    x < y
  * @retval        0     x = y
  * @retval        1     x > y
  */
int __cmp_part(const number_ptr_t& x, const number_ptr_t& y, int cl, int dl) {
  int n, i;
  n = cl - 1;

  //
  // 只要多出来的部分非0，则可以判断两个数字的大小。
  //
  if (dl < 0) {
    for (i = dl; i < 0; i++) {
      if (y[n - i] != 0)
        return -1;      /* x < y */
    }
  }
  else if (dl > 0) {
    for (i = dl; i > 0; i--) {
      if (x[n + i] != 0)
        return 1;       /* x > y */
    }
  }
  return __cmp(x, y, cl);
}