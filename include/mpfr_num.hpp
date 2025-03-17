#pragma once

#include <mpfr.h>
#include <string>
#include <stdexcept>

namespace evqovv {
class mpfr_num {
  friend mpfr_num sqrt(mpfr_num x);

public:
  mpfr_num() { mpfr_init(num); }

  mpfr_num(mpfr_num const &o) {
    mpfr_init(num);
    *this = o;
  }

  mpfr_num(mpfr_num &&o) noexcept {
    mpfr_swap(num, o.num);
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

  mpfr_num &operator+=(mpfr_num const &o) {
    if (auto ret = mpfr_add(num, num, o.num, MPFR_RNDN); ret != 0) {
      throw ret;
    }
    return *this;
  }

  mpfr_num &operator-=(mpfr_num const &o) {
    if (auto ret = mpfr_sub(num, num, o.num, MPFR_RNDN); ret != 0) {
      throw ret;
    }
    return *this;
  }

  mpfr_num &operator*=(mpfr_num const &o) {
    if (auto ret = mpfr_mul(num, num, o.num, MPFR_RNDN); ret != 0) {
      throw ret;
    }
    return *this;
  }

  mpfr_num &operator/=(mpfr_num const &o) {
    if (mpfr_cmp_d(o.num, 0.0) == 0) {
      throw ::std::runtime_error("The divisor is zero.");
    }

    if (auto ret = mpfr_div(num, num, o.num, MPFR_RNDN); ret != 0) {
      throw ret;
    }
    return *this;
  }

private:
  using mpfr_op_t = int (*)(mpfr_t, mpfr_t const, mpfr_t const, mpfr_rnd_t);

  mpfr_num &perform_op(mpfr_op_t op, mpfr_num const &x) {
    if (auto ret = op(num, num, x.num, MPFR_RNDN); ret != 0) {
      throw ret;
    }
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
  mpfr_num t(x);
  if (auto ret = mpfr_sqrt(t.num, x.num, MPFR_RNDN); ret != 0) {
    throw ret;
  }
  return t;
}

} // namespace evqovv