#include <stdint.h>

#include "fraction.h"

static int32_t get_gcd(int32_t a, int32_t b) {
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;

    int32_t r;
    while ((r = a % b) != 0) {
        a = b;
        b = r;
    }

    return b;
}

static int32_t get_lcm(int32_t a, int32_t b) {
    return (a * b) / get_gcd(a, b);
}

fraction frac_reduce(fraction f) {
    if (f.top == f.bottom) {
        f.top = 1;
        f.bottom = 1;
    }

    else if (f.top == 0) {
        f.bottom = 1;
    }

    else {
        int32_t gcd = get_gcd(f.top, f.bottom);

        if (gcd > 1) {
            f.top /= gcd;
            f.bottom /= gcd;
        }

        if ((f.top < 0 && f.bottom < 0) || (f.top >= 0 && f.bottom < 0)) {
            f.top *= -1;
            f.bottom *= -1;
        }
    }

    return f;
}

fraction frac_divide(fraction a, fraction b) {
   a.top *= b.bottom;
   a.bottom *= b.top;

   return frac_reduce(a);
}

fraction frac_multiply(fraction a, fraction b) {
    a.top *= b.top;
    a.bottom *= b.bottom;

    return frac_reduce(a);
}

fraction frac_subtract(fraction a, fraction b) {
    int32_t lcm = get_lcm(a.bottom, b.bottom);
    a.top *= (lcm / a.bottom);
    b.top *= (lcm / b.bottom);

    a.top -= b.top;
    a.bottom = lcm;

    return frac_reduce(a);
}
