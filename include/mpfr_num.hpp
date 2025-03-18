#pragma once

#include <mpfr.h>
#include <string>
#include <stdexcept>

namespace evqovv {
class mpfr_num {
  friend mpfr_num sqrt(mpfr_num x);

public:
  mpfr_num() { mpfr_init(num); }

  mpfr_num(mpfr_num const &o) : mpfr_num() { *this = o; }

  mpfr_num(double v) : mpfr_num() { mpfr_set_d(num, v, MPFR_RNDN); }

  mpfr_num(mpfr_num &&o) noexcept {
    ::mpfr_swap(num, o.num);
    o.is_moved = true;
  }

  mpfr_num(::std::string const &s) : mpfr_num() { set(s); }

  ~mpfr_num() {
    if (!is_moved) {
      mpfr_clear(num);
    }
  }

  mpfr_t const &native() const noexcept { return num; }

  mpfr_num operator-() const {
    mpfr_num t;
    if (auto ret = mpfr_neg(t.num, num, MPFR_RNDN); ret != 0) {
      throw ret;
    }
    return t;
  }

  mpfr_num &operator=(mpfr_num const &o) {
    if (this != &o) {
      mpfr_set(num, o.num, MPFR_RNDN);
    }
    return *this;
  }

  mpfr_num &operator=(mpfr_num &&o) noexcept {
    if (this != &o) {
      mpfr_swap(num, o.num);
      o.is_moved = true;
    }
    return *this;
  }

  mpfr_num &operator=(mpfr_t const &o) {
    mpfr_set(num, o, MPFR_RNDN);
    return *this;
  }

  void set(::std::string const &s) {
    if (auto ret = mpfr_set_str(num, s.c_str(), 10, MPFR_RNDN); ret != 0) {
      throw ret;
    }
  }

  auto operator==(mpfr_num const &x) const -> bool {
    return mpfr_cmp(this->native(), x.native()) == 0;
  }

  auto operator+=(mpfr_num const &o) -> mpfr_num & {
    return perform_op(mpfr_add, o);
  }

  auto operator-=(mpfr_num const &o) -> mpfr_num & {
    return perform_op(mpfr_sub, o);
  }

  auto operator*=(mpfr_num const &o) -> mpfr_num & {
    return perform_op(mpfr_mul, o);
  }

  auto operator/=(mpfr_num const &o) -> mpfr_num & {
    if (o == 0.0) {
      throw ::std::runtime_error("The divisor is zero.");
    }
    return perform_op(mpfr_div, o);
  }

  auto is_integer_number() const noexcept -> bool {
    return mpfr_integer_p(num) == 1;
  }

private:
  using mpfr_op_t = int (*)(mpfr_t, mpfr_t const, mpfr_t const, mpfr_rnd_t);

  auto perform_op(mpfr_op_t op, mpfr_num const &x) -> mpfr_num & {
    op(num, num, x.num, MPFR_RNDN);
    return *this;
  }

  mpfr_t num;
  bool is_moved = false;
};

inline auto operator+(mpfr_num x, mpfr_num const &y) -> mpfr_num {
  return x += y;
}

inline auto operator-(mpfr_num x, mpfr_num const &y) -> mpfr_num {
  return x -= y;
}

inline auto operator*(mpfr_num x, mpfr_num const &y) -> mpfr_num {
  return x *= y;
}

inline auto operator/(mpfr_num x, mpfr_num const &y) -> mpfr_num {
  return x /= y;
}

inline auto sqrt(mpfr_num x) -> mpfr_num {
  mpfr_sqrt(x.num, x.num, MPFR_RNDN);
  return x;
}
} // namespace evqovv