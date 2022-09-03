/* 用h,l除以d并返回结果 */
unit_t __div_unit(unit_t h, unit_t l, unit_t d) {
  unit_t dh, dl, q, ret = 0, th, tl, t;
  int count = 2;

  if (d == 0)
    return CALC_MASK;

  size_t i = __count_bits(d);
  my_assert((i == UNIT_BITS) || (h <= (unit_t)(1 << i)), "invalid word bits, %lu.", i);

  i = UNIT_BITS - i;
  if (h >= d)
    h -= d;

  if (i) {
    d <<= i;
    h = (h << i) | (l >> (UNIT_BITS - i));
    l <<= i;
  }
  dh = (d & CALC_MASKh) >> UNIT_HALF_BITS;
  dl = (d & CALC_MASKl);
  for (;;) {
    if ((h >> UNIT_HALF_BITS) == dh)
      q = CALC_MASKl;
    else
      q = h / dh;

    th = q * dh;
    tl = dl * q;
    for (;;) {
      t = h - th;
      if ((t & CALC_MASKh) || ((tl) <= ((t << UNIT_HALF_BITS) | 
          ((l & CALC_MASKh) >> UNIT_HALF_BITS))))
        break;
      q--;
      th -= dh;
      tl -= dl;
    }
    t = (tl >> UNIT_HALF_BITS);
    tl = (tl << UNIT_HALF_BITS) & CALC_MASKh;
    th += t;

    if (l < tl)
      th++;
    l -= tl;
    if (h < th) {
      h += d;
      q--;
    }
    h -= th;

    if (--count == 0)
      break;

    ret = q << UNIT_HALF_BITS;
    h = ((h << UNIT_HALF_BITS) | (l >> UNIT_HALF_BITS)) & CALC_MASK;
    l = (l & CALC_MASKl) << UNIT_HALF_BITS;
  }
  ret |= q;
  return ret;
}