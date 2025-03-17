// #include <mpfr.h>
#include "application.hpp"
auto main(int argc, char **argv) -> int {
  // mpfr_t t;
  // mpfr_init(t);

  // std::cout << mpfr_set_str(t, "102e", 10, MPFR_RNDN) << '\n';

  evqovv::application app(argc, argv);
  app.run();
}