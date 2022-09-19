std::string __print_string(const number_ptr_t& x, size_t xl) {
  if (xl == 0) return "";
  return "";
}

std::string __print_string(const number_t& x) {
  return __print_string(x.begin(), x.size());
}