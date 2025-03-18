#pragma once

#include <format>
#include "mpfr_env.hpp"

namespace evqovv {
class mpfr_converter {
public:
  mpfr_converter(mpfr_env const &e) noexcept : env(e) {}

  auto operator()(mpfr_num const &num) -> std::string {
    auto fmt = num.is_integer_number() ? get_integer_format_string()
                                       : get_floating_format_string();

    auto sz = mpfr_snprintf(nullptr, 0, fmt.c_str(), num.native());
    ++sz; // for '\0'.

    auto buf = std::make_unique_for_overwrite<char[]>(sz);
    mpfr_snprintf(buf.get(), sz, fmt.c_str(), num.native());

    return buf.get();
  }

private:
  auto get_floating_format_string() const -> ::std::string {
    return std::format("%.{}Rf", env.get_print_precision());
  }

  auto get_integer_format_string() const -> ::std::string {
    return env.get_keep_decimal() ? get_floating_format_string() : "%.0Rf";
  }

  mpfr_env const &env;
};
} // namespace evqovv