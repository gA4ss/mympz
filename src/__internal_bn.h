//
// 这里的number_t是内部大数所用的结构，一个unit_t表示一个10进制数
// 效率低下，仅作转换使用。
//
typedef std::pair<number_t, number_t> internal_division_result_t;
bool __ibn_is_zero(const number_t& a);
void __ibn_zero(number_t& a);
size_t __ibn_shrink_zero(number_t& a, bool reverse=false);
int __ibn_cmp(const number_t& a, const number_t& b, bool push_front=false);
number_t __ibn_add(const number_t& a, const number_t& b, bool o);
number_t __ibn_sub(number_t& a, number_t& b, bool t);
number_t __ibn_mul(const number_t& a, const number_t& b);
internal_division_result_t __ibn_div(const number_t& a, const number_t& b);

//
// 这里的number_t即为标准的大数结构
//
number_t __string_to_bignum(const char* number);
std::string __bignum_to_string(const number_ptr_t& a, size_t al);
