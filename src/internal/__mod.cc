/**
  * @brief         有符号的两个大数的模数相加取模后取绝对值
  * @param[in]     x x >=0 && x < m
  * @param[in]     y y >=0 && y < m
  * @return        r = |(x + y) % m|
  */
number_t __mod_add(const number_ptr_t& x, size_t xl, const number_ptr_t& y, size_t yl,
                   const number_ptr_t& m, size_t ml) {
  number_t storage; num_resize(storage, 1024 / UNIT_BITS);
  if (ml > num_size(storage)) {
    num_resize(storage, ml);
  }
  number_ptr_t tp = num_ptr(storage);
  number_t r; num_resize(r, ml);

  unit_t carry = 0, temp = 0, mask = 0;
  for (size_t i = 0, xi = 0, yi = 0; i < ml;) {
    mask = (unit_t)0 - ((i - xl) >> (8 * sizeof(i) - 1));
    temp = ((x[xi] & mask) + carry) & CALC_MASK;
    carry = (temp < carry);

    mask = (unit_t)0 - ((i - yl) >> (8 * sizeof(i) - 1));
    tp[i] = ((y[yi] & mask) + temp) & CALC_MASK;
    carry += (tp[i] < temp);

    i++;
    xi += (i - xl) >> (8 * sizeof(i) - 1);
    yi += (i - yl) >> (8 * sizeof(i) - 1);
  }

  number_ptr_t rp = num_ptr(r);
  carry -= __sub_units(rp, tp, m, ml);
  for (size_t i = 0; i < ml; i++) {
    rp[i] = (carry & tp[i]) | (~carry & rp[i]);
    tp[i] = 0;
  }

  return r;
}


/**
  * @brief         有符号的两个大数的模数相减取模后取绝对值
  * @param[in]     x x >=0 && x < m
  * @param[in]     y y >=0 && y < m
  * @return        r = |(x - y) % m|
  */
number_t __mod_sub(const number_ptr_t& x, size_t xl, const number_ptr_t& y, size_t yl,
                   const number_ptr_t& m, size_t ml) {
  number_t r; num_resize(r, ml);

  unit_t tx, ty;
  unit_t borrow = 0, carry = 0, mask = 0;
  for (size_t i = 0, xi = 0, yi = 0; i < ml;) {
    mask = (unit_t)0 - ((i - xl) >> (8 * sizeof(i) - 1));
    tx = x[xi] & mask;

    mask = (unit_t)0 - ((i - yl) >> (8 * sizeof(i) - 1));
    ty = y[yi] & mask;
    r[i] = tx - ty - borrow;
    if (tx != ty)
      borrow = (tx < ty);

    i++;
    xi += (i - xl) >> (8 * sizeof(i) - 1);
    yi += (i - yl) >> (8 * sizeof(i) - 1);
  }

  mask = 0 - borrow; carry = 0;
  for (size_t i = 0; i < ml; i++) {
    tx = ((m[i] & mask) + carry) & CALC_MASK;
    carry = (tx < carry);
    r[i] = (r[i] + tx) & CALC_MASK;
    carry += (r[i] < tx);
  }

  borrow -= carry;
  mask = 0 - borrow; carry = 0;
  for (size_t i = 0; i < ml; i++) {
    tx = ((m[i] & mask) + carry) & CALC_MASK;
    carry = (tx < carry);
    r[i] = (r[i] + tx) & CALC_MASK;
    carry += (r[i] < tx);
  }

  return r;
}