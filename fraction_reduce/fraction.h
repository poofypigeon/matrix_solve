#ifndef FRACTION_H
#define FRACTION_H

#include <stdint.h>

typedef struct {
    int32_t top;
    int32_t bottom;
} fraction;

fraction frac_reduce(fraction f);

fraction frac_divide(fraction a, fraction b);

fraction frac_multiply(fraction a, fraction b);

fraction frac_subtract(fraction a, fraction b);

#endif // FRACTION_H