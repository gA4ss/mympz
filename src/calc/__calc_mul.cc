void __mul_4_units(number_t& z, const number_t& x, const number_t& y) {
  unit_t c1 = 0, c2 = 0, c3 = 0;

  mul_add_c(x[0], y[0], c1, c2, c3);
  z[0] = c1;
  c1 = 0;
  mul_add_c(x[0], y[1], c2, c3, c1);
  mul_add_c(x[1], y[0], c2, c3, c1);
  z[1] = c2;
  c2 = 0;
  mul_add_c(x[2], y[0], c3, c1, c2);
  mul_add_c(x[1], y[1], c3, c1, c2);
  mul_add_c(x[0], y[2], c3, c1, c2);
  z[2] = c3;
  c3 = 0;
  mul_add_c(x[0], y[3], c1, c2, c3);
  mul_add_c(x[1], y[2], c1, c2, c3);
  mul_add_c(x[2], y[1], c1, c2, c3);
  mul_add_c(x[3], y[0], c1, c2, c3);
  z[3] = c1;
  c1 = 0;
  mul_add_c(x[3], y[1], c2, c3, c1);
  mul_add_c(x[2], y[2], c2, c3, c1);
  mul_add_c(x[1], y[3], c2, c3, c1);
  z[4] = c2;
  c2 = 0;
  mul_add_c(x[2], y[3], c3, c1, c2);
  mul_add_c(x[3], y[2], c3, c1, c2);
  z[5] = c3;
  c3 = 0;
  mul_add_c(x[3], y[3], c1, c2, c3);
  z[6] = c1;
  z[7] = c2;
}

void __mul_8_units(number_t& z, const number_t& x, const number_t& y) {
  unit_t c1 = 0, c2 = 0, c3 = 0;

  mul_add_c(x[0], y[0], c1, c2, c3);
  z[0] = c1;
  c1 = 0;
  mul_add_c(x[0], y[1], c2, c3, c1);
  mul_add_c(x[1], y[0], c2, c3, c1);
  z[1] = c2;
  c2 = 0;
  mul_add_c(x[2], y[0], c3, c1, c2);
  mul_add_c(x[1], y[1], c3, c1, c2);
  mul_add_c(x[0], y[2], c3, c1, c2);
  z[2] = c3;
  c3 = 0;
  mul_add_c(x[0], y[3], c1, c2, c3);
  mul_add_c(x[1], y[2], c1, c2, c3);
  mul_add_c(x[2], y[1], c1, c2, c3);
  mul_add_c(x[3], y[0], c1, c2, c3);
  z[3] = c1;
  c1 = 0;
  mul_add_c(x[4], y[0], c2, c3, c1);
  mul_add_c(x[3], y[1], c2, c3, c1);
  mul_add_c(x[2], y[2], c2, c3, c1);
  mul_add_c(x[1], y[3], c2, c3, c1);
  mul_add_c(x[0], y[4], c2, c3, c1);
  z[4] = c2;
  c2 = 0;
  mul_add_c(x[0], y[5], c3, c1, c2);
  mul_add_c(x[1], y[4], c3, c1, c2);
  mul_add_c(x[2], y[3], c3, c1, c2);
  mul_add_c(x[3], y[2], c3, c1, c2);
  mul_add_c(x[4], y[1], c3, c1, c2);
  mul_add_c(x[5], y[0], c3, c1, c2);
  z[5] = c3;
  c3 = 0;
  mul_add_c(x[6], y[0], c1, c2, c3);
  mul_add_c(x[5], y[1], c1, c2, c3);
  mul_add_c(x[4], y[2], c1, c2, c3);
  mul_add_c(x[3], y[3], c1, c2, c3);
  mul_add_c(x[2], y[4], c1, c2, c3);
  mul_add_c(x[1], y[5], c1, c2, c3);
  mul_add_c(x[0], y[6], c1, c2, c3);
  z[6] = c1;
  c1 = 0;
  mul_add_c(x[0], y[7], c2, c3, c1);
  mul_add_c(x[1], y[6], c2, c3, c1);
  mul_add_c(x[2], y[5], c2, c3, c1);
  mul_add_c(x[3], y[4], c2, c3, c1);
  mul_add_c(x[4], y[3], c2, c3, c1);
  mul_add_c(x[5], y[2], c2, c3, c1);
  mul_add_c(x[6], y[1], c2, c3, c1);
  mul_add_c(x[7], y[0], c2, c3, c1);
  z[7] = c2;
  c2 = 0;
  mul_add_c(x[7], y[1], c3, c1, c2);
  mul_add_c(x[6], y[2], c3, c1, c2);
  mul_add_c(x[5], y[3], c3, c1, c2);
  mul_add_c(x[4], y[4], c3, c1, c2);
  mul_add_c(x[3], y[5], c3, c1, c2);
  mul_add_c(x[2], y[6], c3, c1, c2);
  mul_add_c(x[1], y[7], c3, c1, c2);
  z[8] = c3;
  c3 = 0;
  mul_add_c(x[2], y[7], c1, c2, c3);
  mul_add_c(x[3], y[6], c1, c2, c3);
  mul_add_c(x[4], y[5], c1, c2, c3);
  mul_add_c(x[5], y[4], c1, c2, c3);
  mul_add_c(x[6], y[3], c1, c2, c3);
  mul_add_c(x[7], y[2], c1, c2, c3);
  z[9] = c1;
  c1 = 0;
  mul_add_c(x[7], y[3], c2, c3, c1);
  mul_add_c(x[6], y[4], c2, c3, c1);
  mul_add_c(x[5], y[5], c2, c3, c1);
  mul_add_c(x[4], y[6], c2, c3, c1);
  mul_add_c(x[3], y[7], c2, c3, c1);
  z[10] = c2;
  c2 = 0;
  mul_add_c(x[4], y[7], c3, c1, c2);
  mul_add_c(x[5], y[6], c3, c1, c2);
  mul_add_c(x[6], y[5], c3, c1, c2);
  mul_add_c(x[7], y[4], c3, c1, c2);
  z[11] = c3;
  c3 = 0;
  mul_add_c(x[7], y[5], c1, c2, c3);
  mul_add_c(x[6], y[6], c1, c2, c3);
  mul_add_c(x[5], y[7], c1, c2, c3);
  z[12] = c1;
  c1 = 0;
  mul_add_c(x[6], y[7], c2, c3, c1);
  mul_add_c(x[7], y[6], c2, c3, c1);
  z[13] = c2;
  c2 = 0;
  mul_add_c(x[7], y[7], c3, c1, c2);
  z[14] = c3;
  z[15] = c1;
}

unit_t __mul_units_unit(number_t& z, const number_t &x, const unit_t& y) {
  size_t n = x.size();
  if (n == 0)
    return 0;

  unit_t yl = LBITS(y);
  unit_t yh = HBITS(y);

  unit_t carry = 0;
  size_t i = 0;
#ifndef SMALL_FOOTPRINT
  while (n & ~3) {
    mul(z[i], x[i], yl, yh, carry);
    mul(z[i+1], x[i+1], yl, yh, carry);
    mul(z[i+2], x[i+2], yl, yh, carry);
    mul(z[i+3], x[i+3], yl, yh, carry);
    i += 4;
    n -= 4;
  }
#endif
  while (n) {
    mul(z[i], x[i], yl, yh, carry);
    i++;
    n--;
  }
  return carry;
}

unit_t __mul_add_units(number_t& z, size_t st, const number_t &x, size_t num, const unit_t& y) {
  unit_t c = 0;
  unit_t yl, yh;

  my_assert(num >= 0);
  if (num == 0)
    return 0;

  unit_t yl = LBITS(y);
  unit_t yh = HBITS(y);
  size_t i = 0;

#ifndef SMALL_FOOTPRINT
  while (num & ~3) {
    mul_add(z[st+i], x[i], yl, yh, c);
    mul_add(z[st+i+1], x[i+1], yl, yh, c);
    mul_add(z[st+i+2], x[i+2], yl, yh, c);
    mul_add(z[st+i+3], x[i+3], yl, yh, c);
    i += 4;
    num -= 4;
  }
# endif

  while (num) {
    mul_add(z[i], x[i], yl, yh, c);
    i++;
    num--;
  }
  return c;
}

void __mul_units_loop(number_t& z, const number_t& x, const number_t& y) {
  unit_t *rr;
  size_t nx = bn_size(x), ny = bn_size(y);

  //
  // 如果x的长度小于y,则交换两个大数队列。
  //
  const number_t& _x;
  const number_t& _y;
  if (nx < ny) {
    _x = y;
    _y = x;
    nx = bn_size(_x);
    ny = bn_size(_y);
  } else {
    _x = x;
    _y = y;
  }

  if (ny == 0) {
    z = _x;   // 将x赋给z
    return;
  }

  //
  // 进位保存在nx的索引，其余的保存在z的[0, nx-1]范围中。
  //
  size_t i = nx, j = 0, k = 0;
  z[i] = __mul_units_unit(z, _x, _y[j]);
  for (;;) {
    if (--ny == 0)
      return;
    z[i+1] = __mul_add_units(z, k+1, _x, nx, _y[j+1]);
    if (--ny == 0)
      return;
    z[i+2] = __mul_add_units(z, k+2, _x, nx, _y[j+2]);
    if (--ny == 0)
      return;
    z[i+3] = __mul_add_units(z, k+3, _x, nx, _y[j+3]);
    if (--ny == 0)
      return;
    z[i+4] = __mul_add_units(z, k+4, _x, nx, _y[j+4]);
    i += 4;
    j += 4;
    k += 4;
  }
}