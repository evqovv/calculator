#pragma once

#include <numeric>
#include <vector>
#include <ranges>
#include <iostream>

#include "parser.hpp"

namespace evqovv {
class application {
public:
  application(int argc, char **argv) : args(argv + 1, argv + argc) {}

  auto run() -> void {
    if (args.empty()) {
      cmd();
    } else {
      evaluate_expression(concatenate_str());
    }
  }

private:
  auto cmd() -> void {
    ::std::string input;
    while (true) {
      ::std::cout << "-> ";
      ::std::getline(::std::cin, input);

      evaluate_expression(input);
    }
  }

  auto execute_cmd(::std::string const &cmd) -> void {
    auto parts = cmd | ::std::views::split(' ') |
                 ::std::views::transform([](auto &&s) {
                   return ::std::string(s.begin(), s.end());
                 }) |
                 ::std::ranges::to<::std::vector>();
    if (parts.size() == 1) {

    } else {
    }
  }

  auto evaluate_expression(::std::string_view expr) -> void {
    parser p(expr);

    try {
      auto ret = p.parse_expression();
      mpfr_printf("Ret: %.10Rf\n", ret.native());
    } catch (::std::exception const &e) {
      ::std::cout << e.what() << '\n';
    }
  }

  auto concatenate_str() -> ::std::string {
    return ::std::accumulate(args.cbegin(), args.cend(), ::std::string{});
  }

  static void exit() { ::std::exit(0); }

  ::std::vector<::std::string> args;
  ::std::unordered_map<::std::string, ::std::function<void()>> ft = {
      {"quit", exit},
      {"exit", exit},
  };
};
} // namespace evqovv