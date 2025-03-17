#pragma once

#include "mpfr_num.hpp"

namespace evqovv {
class mpfr_formatter {
public:
  mpfr_formatter(unsigned int p = 10, bool a = false) noexcept
      : precision(p), always_show_trailing_zero(a) {}

  auto operator()(mpfr_num const &num) -> std::string {
    auto f_str =
        always_show_trailing_zero ? std::format("%.{}Rf", precision) : "%.Zd";

    auto sz = mpfr_snprintf(nullptr, 0, f_str.c_str(), num.native());
    ++sz;

    auto buf = std::make_unique_for_overwrite<char[]>(sz);
    mpfr_snprintf(buf.get(), sz, f_str.c_str(), num.native());

    return buf.get();
  }

private:
  unsigned int precision = 10;
  bool always_show_trailing_zero = false;
};
} // namespace evqovv