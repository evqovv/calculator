#pragma once

#include <string>
#include <variant>

#include <mpfr_num.hpp>

namespace evqovv {
enum class token_kind : unsigned char {
  number,
  function,
  addition,
  subtraction,
  multiplication,
  division,
  left_parenthesis,
  right_parenthesis,
  end,
};

struct token {
  token_kind kind;
  std::variant<std::string, mpfr_num> data;
};

class lexer {
public:
  explicit lexer(std::string_view e)
      : expr(e), cur_pos(expr.cbegin()), prev_pos(cur_pos) {}

  auto rewind_to_previous_token() { cur_pos = prev_pos; }

  auto get_token() -> token {
    prev_pos = cur_pos;

    skip_whitespace();

    if (cur_pos == expr.cend()) {
      return {token_kind::end};
    }

    if (std::isdigit(*cur_pos) || *cur_pos == '.') {
      return {token_kind::number, get_number()};
    }

    if (std::isalpha(*cur_pos)) {
      return {token_kind::function, get_word()};
    }

    auto ch = *cur_pos++;
    switch (ch) {
    case '+':
      return {token_kind::addition};
    case '-':
      return {token_kind::subtraction};
    case '*':
      return {token_kind::multiplication};
    case '/':
      return {token_kind::division};
    case '(':
      return {token_kind::left_parenthesis};
    case ')':
      return {token_kind::right_parenthesis};
    default:
      throw std::runtime_error("Unknown token.");
    }
  }

private:
  auto skip_whitespace() noexcept -> void {
    for (; cur_pos != expr.cend() && std::isspace(*cur_pos); ++cur_pos) {
    }
  }

  auto get_number() -> mpfr_num {
    bool after_decimal_point = false;
    bool after_exponent = false;
    bool exponent_sign_bit = false;

    auto begin_pos = cur_pos;
    for (; cur_pos != expr.cend(); ++cur_pos) {
      if (isdigit(*cur_pos)) {
        exponent_sign_bit = false;
        continue;
      }

      if (*cur_pos == '.' && !after_decimal_point && !after_exponent) {
        after_decimal_point = true;
        continue;
      }

      if ((*cur_pos == 'e' || *cur_pos == 'E') && !after_exponent) {
        after_exponent = true;
        exponent_sign_bit = true;
        continue;
      }

      if ((*cur_pos == '+' || *cur_pos == '-') && exponent_sign_bit) {
        exponent_sign_bit = false;
        continue;
      }

      break;
    }
    return {std::string(begin_pos, cur_pos)};
  }

  auto get_word() -> std::string {
    auto begin_pos = cur_pos;
    cur_pos = std::find_if(begin_pos, expr.cend(),
                           [](auto &&ch) -> bool { return !std::isalpha(ch); });
    return {begin_pos, cur_pos};
  }

  std::string expr;
  std::string::const_iterator cur_pos;
  std::string::const_iterator prev_pos;
};
} // namespace evqovv