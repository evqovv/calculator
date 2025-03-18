#pragma once

#include <numeric>
#include <vector>
#include <ranges>
#include <iostream>

#include "parser.hpp"
#include "mpfr_converter.hpp"

namespace evqovv {
class application {
public:
  application(int argc, char **argv) : args(argv + 1, argv + argc) {}

  auto run() -> void {
    if (args.empty()) {
      cmd();
    } else {
      try {
        std::print("{}\n", converter(concatenate_str()));
      } catch (std::exception const &e) {
        std::print("{}\n", e.what());
      }
    }
  }

private:
  auto cmd() -> void {
    std::string input;
    while (true) {
      std::print("-> ");
      std::getline(std::cin, input);

      try {
        execute_cmd(input);

        std::print("{}\n", converter(evaluate_expression(input)));
      } catch (std::exception const &e) {
        std::print("{}\n", e.what());
      }
    }
  }

  auto execute_cmd(std::string const &cmd) -> void {
    auto parts = cmd | std::views::split(' ') |
                 std::views::transform(
                     [](auto &&s) { return std::string(s.begin(), s.end()); }) |
                 std::ranges::to<std::vector>();
    if (parts.size() == 1) {

    } else if (parts.size() == 3) {
      if (parts[0] == "set") {
        env.set(parts[1], parts[2]);
      }
    }
  }

  auto evaluate_expression(std::string_view expr) -> mpfr_num {
    return parser(expr).parse_expression();
  }

  auto concatenate_str() -> std::string {
    return std::accumulate(args.cbegin(), args.cend(), std::string{});
  }

  static auto exit() -> void { std::exit(0); }

  mpfr_env env;
  mpfr_converter converter{env};
  std::vector<std::string> args;
  std::unordered_map<std::string, std::function<void()>> const ft{
      {"quit", exit},
      {"exit", exit},
  };
};
} // namespace evqovv