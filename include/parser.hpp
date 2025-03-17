#pragma once

#include <unordered_map>
#include <functional>

#include "lexer.hpp"

namespace evqovv {
class parser {
public:
  parser(::std::string_view expr) : l(expr) { next_token(); }

  auto parse_expression() -> mpfr_num;

private:
  auto parse_unary() -> mpfr_num {
    auto kind = cur_token.kind;

    next_token();
    if (cur_token.kind == token_kind::addition ||
        cur_token.kind == token_kind::subtraction ||
        cur_token.kind == token_kind::multiplication ||
        cur_token.kind == token_kind::division) {
      throw ::std::runtime_error("Parsing failed.");
    }

    return (kind == token_kind::subtraction) ? -parse_factor() : parse_factor();
  }

  auto parse_number() -> mpfr_num {
    return ::std::get<mpfr_num>(cur_token.data);
  }

  auto parse_function() -> mpfr_num {
    auto func_name = ::std::get<::std::string>(cur_token.data);
    if (!ft.contains(func_name)) {
      throw ::std::runtime_error("");
    }

    next_token();

    return ft.at(func_name)(parse_expression());
  }

  auto parse_parentheses() -> mpfr_num {
    next_token();

    auto x = parse_expression();

    if (cur_token.kind != token_kind::right_parenthesis) {
      throw ::std::runtime_error("Parsing failed.");
    }

    return x;
  }

  auto parse_term() -> mpfr_num;

  auto parse_factor() -> mpfr_num;

  auto next_token() -> void { cur_token = l.get_token(); }

  lexer l;
  token cur_token;

  const ::std::unordered_map<::std::string, ::std::function<mpfr_num(mpfr_num)>>
      ft{
          {"sqrt", sqrt},
      };
};

inline auto parser::parse_expression() -> mpfr_num {
  auto x = parse_term();

  while (cur_token.kind == token_kind::addition ||
        cur_token.kind == token_kind::subtraction) {
    auto kind = cur_token.kind;

    next_token();

    auto y = parse_term();

    if (kind == token_kind::addition) {
      x += y;
    } else {
      x -= y;
    }
  }

  return x;
}

inline auto parser::parse_term() -> mpfr_num {
  auto x = parse_factor();

  while (cur_token.kind == token_kind::multiplication ||
        cur_token.kind == token_kind::division) {
    auto kind = cur_token.kind;

    next_token();

    auto y = parse_factor();

    if (kind == token_kind::multiplication) {
      x *= y;
    } else {
      x /= y;
    }
  }

  return x;
}

inline auto parser::parse_factor() -> mpfr_num {
  mpfr_num x;

  switch (cur_token.kind) {
  case token_kind::addition:
  case token_kind::subtraction:
    x = parse_unary();
    break;
  case token_kind::number:
    x = parse_number();
    break;
  case token_kind::left_parenthesis:
    x = parse_parentheses();
    break;
  case token_kind::function:
    x = parse_function();
    break;
  default:
    throw ::std::runtime_error("Parsing failed.");
    break;
  }

  next_token();
  return x;
}
} // namespace evqovv