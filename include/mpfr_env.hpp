#pragma once

#include <unordered_map>
#include <string_view>
#include <functional>
#include <format>
#include "mpfr_num.hpp"

namespace evqovv {
class mpfr_env {
public:
  mpfr_env() {}

  auto get_default_precision() const noexcept -> unsigned int {
    return default_precision;
  }

  auto get_print_precision() const noexcept -> unsigned int {
    return print_precision;
  }

  auto get_keep_decimal() const noexcept -> bool {
    return keep_decimal;
  }

  auto set(std::string_view key, std::string_view value) -> void {
    if (auto func = ft.find(std::string(key)); func != ft.cend()) {
      func->second(value);
    } else {
      throw std::runtime_error(std::format("{} can't be found.", key));
    }
  }

private:
  unsigned int default_precision = 50;
  unsigned int print_precision = 50;
  bool keep_decimal = false;

  std::unordered_map<std::string, std::function<void(std::string_view)>> const
      ft{
          {"precision",
           [this](std::string_view val) {
             default_precision = std::stoi(std::string(val));
             print_precision = default_precision;
             mpfr_set_default_prec(default_precision);
           }},
          {"default_precision",
           [this](std::string_view val) {
             default_precision = std::stoi(std::string(val));
             mpfr_set_default_prec(default_precision);
           }},
          {"print_precision",
           [this](std::string_view val) {
             print_precision = std::stoi(std::string(val));
           }},
          {"keep_decimal",
           [this](std::string_view val) {
             keep_decimal = (val == "true" || val == "1");
           }},
      };
};
} // namespace evqovv