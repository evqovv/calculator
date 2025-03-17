#pragma once

#include <numeric>
#include <vector>
#include <ranges>
#include <iostream>

#include "parser.hpp"
#include "mpfr_formatter.hpp"

namespace evqovv {
class application {
public:
  application(int argc, char **argv) : args(argv + 1, argv + argc) {}

  auto run() -> void {
    if (args.empty()) {
      cmd();
    } else {
      std::print("{}\n", formatter(concatenate_str()));
    }
  }

private:
  auto cmd() -> void {
    std::string input;
    while (true) {
      std::print("-> ");
      std::getline(std::cin, input);

      std::print("{}\n", formatter(evaluate_expression(input)));
    }
  }

  auto execute_cmd(std::string const &cmd) -> void {
    auto parts = cmd | std::views::split(' ') |
                 std::views::transform(
                     [](auto &&s) { return std::string(s.begin(), s.end()); }) |
                 std::ranges::to<std::vector>();
    if (parts.size() == 1) {

    } else {
    }
  }

  auto evaluate_expression(std::string_view expr) -> mpfr_num {
    return parser(expr).parse_expression();
  }

  auto concatenate_str() -> std::string {
    return std::accumulate(args.cbegin(), args.cend(), std::string{});
  }

  static auto exit() -> void { std::exit(0); }

  mpfr_formatter formatter;
  std::vector<std::string> args;
  std::unordered_map<std::string, std::function<void()>> ft = {
      {"quit", exit},
      {"exit", exit},
  };
};
} // namespace evqovv