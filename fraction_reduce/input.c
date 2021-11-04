#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

#include "input.h"
#include "fraction.h"

int parse_int_32(int32_t *dest, char const digits[12], uint8_t size, bool negative) {
    uint64_t parsed_digits = 0;
    uint64_t multiplier = 1;

    for (uint8_t i = 0; i < size; i++) {
        parsed_digits += (digits[size - i - 1] - '0') * multiplier;
        multiplier *= 10;
    }

    if (negative) {
        parsed_digits = -parsed_digits;
        if (parsed_digits < (int64_t) INT32_MIN) {
            return -1;
        }
    }

    else {
        if (parsed_digits > (int64_t) INT32_MAX) {
            return 1;
        }
    }

    *dest = parsed_digits;
    return 0;
}


int read_int_32(char buffer[12], char *ch) {
    int digits = 0;

    if (!isnumber(*ch)) {
        do { 
            buffer[digits] = *ch; 
            digits++;
        } while (!isspace(*ch = getchar()) && digits < 11);
        buffer[digits] = '\0';

        return -digits;
    }
    
    do {
        buffer[digits] = *ch;
        digits++;
    } while (isnumber(*ch = getchar()) && digits < 11);
    buffer[digits] = '\0';

    if (isnumber(*ch)) {
        return 0;
    }

    return digits;
}

void read_entry(fraction* entry) {
    char numerator[11];
    char denominator[11];
    int8_t numerator_digits = 0;
    int8_t denominator_digits = 0;

    bool is_negative = false;

    char ch;

    // scan past whitespace
    do { ch = getchar(); } while (isspace(ch));

    // test for negative numerator
    if (ch == '-') {
        is_negative = true;
        ch = getchar();
    }

    // scan for denominator
    numerator_digits = read_int_32(numerator, &ch);

    // too many digits
    if (numerator_digits == 0) {
        printf("A numerator is too large.\n");
        exit(1);
    }

    // not an integer input
    if (numerator_digits < 0) {
        printf("Invalid coefficient: '%s%s'.\n", numerator, (numerator_digits == -11) ? "..." : "");
        exit(1);
    }

    // parse numerator and write to entry->top
    if (parse_int_32(&(entry->top), numerator, numerator_digits, is_negative) != 0) {
        // outside representable int32_t values
        printf("A numerator is too large.\n");
        exit(1);
    }

    // test for presence of a denominator
    if (ch == '/') {
        ch = getchar();
        // scan for denominator
        denominator_digits = read_int_32(denominator, &ch);

        // too many digits
        if (denominator_digits == 0) {
            printf("A denominator is too large.\n");
            exit(1);
        }

        // not an integer input
        if (denominator_digits < 0) {
            printf("Invalid coefficient: '%s%s'.\n", denominator, (denominator_digits == -11) ? "..." : "");
            exit(1);
        }

        // parse numerator and write to entry->top
        if (parse_int_32(&(entry->bottom), denominator, denominator_digits, false) != 0) {
            // outside representable int32_t values
            printf("A denominator is too large.\n");
            exit(1);
        }
    } 
    
    else {
        // coefficient is an integer
        entry->bottom = 1;
    }
}
