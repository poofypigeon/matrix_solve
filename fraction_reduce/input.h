#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#include "fraction.h"

int parse_int_32(int32_t *dest, char const digits[12], uint8_t size, bool negative);

int read_int_32 (char buffer[12], char *ch);

void read_entry(fraction* entry);

#endif // INPUT_H