def generate_polynomial(num, r):
    bstr = bin(num)[2:]
    coefficients = []

    if len(bstr) % r == 0:
        u = len(bstr)//r
    else:
        u = len(bstr)//r+1
        bstr = '0' * (u * r - len(bstr)) + bstr

    j = 0
    for i in range(0, r):
        c = bstr[j:j+u]
        coefficients.append(c)
        j += u
    return list(map(lambda x: int(x, 2), coefficients))


def eval_poly(poly, x):
    return sum([poly[i] * x**(len(poly)-1-i) for i in range(0, len(poly))])


def eval_w(u_poly, v_poly, n):
    u = [eval_poly(u_poly, i) for i in range(0, n)]
    v = [eval_poly(v_poly, i) for i in range(0, n)]
    w = list(map(lambda x, y: x*y, u, v))
    return w


def generate_decline_w_poly(w, k=0, first=True):
    w_poly = []
    if first:
        col = w
    else:
        col = [w[i]//k for i in range(0, len(w))]
    w_poly.append(col[0])

    if (len(w) > 1):
        next_col = [col[i+1]-col[i] for i in range(0, len(col)-1)]
        w_poly.extend(generate_decline_w_poly(next_col, k+1, False))

    if first:
        w_poly = w_poly[::-1]
    return w_poly


def generate_w_poly(w):
    def __eval_c(x, y_list, k):
        res = []
        y = y_list[k]

        res.append(x)
        if k == 1:
          res.append(x-k*y)
          return res

        res.extend(__eval_c(x-k*y, y_list, k-1))
        return res

    w_poly = []
    dec_w_poly = generate_decline_w_poly(w)
    w_poly.append(dec_w_poly[0])

    m = len(dec_w_poly)
    k = m-2
    y_list = [dec_w_poly[0]] * (m-1)
    for i in range(1, m-1):
        y_list = __eval_c(dec_w_poly[i], y_list, k)
        w_poly.append(y_list[-1])
        y_list = y_list[::-1]
        k -= 1

    w_poly.append(dec_w_poly[-1])
    return w_poly


u_poly = generate_polynomial(11111, 3)
print(u_poly)
v_poly = generate_polynomial(22222, 3)
print(v_poly)
w = eval_w(u_poly, v_poly, 5)
print(w)
w_poly = generate_w_poly(w)
print(w_poly)

for r in range(2,10):
  res = eval_poly(w_poly, 2**r)
  print("{}:{}".format(r,res))

# u = []
# v = []
# w = []
# for i in range(0, 5):
#   u.append(U(i))
#   v.append(V(i))
#   w.append(U(i) * V(i))

# print(u)
# print(v)
# print(w)
