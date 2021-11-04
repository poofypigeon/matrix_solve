#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

typedef struct {
    int32_t top;
    int32_t bottom;
} fraction;

int32_t get_gcd(int32_t a, int32_t b) {
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;

    int32_t r;
    while ((r = a % b) != 0) {
        a = b;
        b = r;
    }

    return b;
}

int32_t get_lcm(int32_t a, int32_t b) {
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

void reduce_matrix (fraction **matrix, size_t x_size, size_t y_size) {
    size_t rank = 0;

    putchar('\n');
    for (int y = 0; y < y_size; y++) {
        for (int x = 0; x < x_size; x++) {
            printf("%d/%d ", matrix[y][x].top, matrix[y][x].bottom);
        }
        putchar('\n');
    }

    for (size_t col = 0; col < x_size; col++) {
        bool leading_entry_found = false;

        for (size_t row = rank; row < y_size; row++) {
            if (matrix[row][col].top != 0) {
                if (row != rank) {
                    fraction *temp = matrix[rank];
                    matrix[rank] = matrix[row];
                    matrix[row] = temp;
                }

                fraction leading_value = {
                    matrix[rank][col].top, 
                    matrix[rank][col].bottom
                };

                if (!(leading_value.top == 1 && leading_value.bottom == 1)) {
                    for (size_t i = col; i < x_size; i++) {
                        matrix[rank][i] = frac_divide(matrix[rank][i], leading_value);
                    }
                }

                leading_entry_found = true;
                break;
            }
        }

        if (leading_entry_found) {
            for (size_t row = 0; row < y_size; row++) {
                if (row == rank) continue;

                if (matrix[row][col].top != 0) {
                    fraction leading_value = {
                        matrix[row][col].top, 
                        matrix[row][col].bottom
                    };

                    for (size_t i = col; i < x_size; i++) {
                        matrix[row][i] = frac_subtract(
                            matrix[row][i], 
                            frac_multiply(matrix[rank][i], leading_value)
                        );
                    }
                }
            }

            rank++;
        }
    }
}

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


int read_int_32 (char buffer[12], char *ch) {
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
    do { ch = getchar(); } while (isspace(ch));

    if (ch == '-') {
        is_negative = true;
        ch = getchar();
    }

    numerator_digits = read_int_32(numerator, &ch);

    if (numerator_digits == 0) {
        printf("A numerator is too large.\n");
        exit(1);
    }

    if (numerator_digits < 0) {
        printf("Invalid coefficient: '%s%s'.\n", numerator, (numerator_digits == -11) ? "..." : "");
        exit(1);
    }

    if (parse_int_32(&(entry->top), numerator, numerator_digits, is_negative) != 0) {
        printf("A numerator is too large.\n");
        exit(1);
    }

    if (ch == '/') {
        ch = getchar();
        denominator_digits = read_int_32(denominator, &ch);

        if (denominator_digits == 0) {
            printf("A denominator is too large.\n");
            exit(1);
        }

        if (denominator_digits < 0) {
            printf("Invalid coefficient: '%s%s'.\n", denominator, (denominator_digits == -11) ? "..." : "");
            exit(1);
        }

        if (parse_int_32(&(entry->bottom), denominator, denominator_digits, false) != 0) {
            printf("A denominator is too large.\n");
            exit(1);
        }
    } 
    
    else {
        entry->bottom = 1;
    }

}

int main() {
    size_t x_size, y_size;

    printf("Rows: ");
    scanf("%zu", &y_size);

    printf("Columns: ");
    scanf("%zu", &x_size);

    fraction **matrix = calloc(y_size, sizeof(fraction *));

    for (int i = 0; i < y_size; i++) {
        matrix[i] = calloc(x_size, sizeof(fraction));
    }

    printf("Entries:\n");
    for (size_t y = 0; y < y_size; y++) {
        for (size_t x = 0; x < x_size; x++) {
            read_entry(&matrix[y][x]);
        }
    }

    reduce_matrix(matrix, x_size, y_size);

    putchar('\n');
    for (size_t y = 0; y < y_size; y++) {
        for (size_t x = 0; x < x_size; x++) {
            if (matrix[y][x].top == 0 || matrix[y][x].bottom == 1)
                printf("%d ", matrix[y][x].top);
            else
                printf("%d/%d ", matrix[y][x].top, matrix[y][x].bottom);
        }
        putchar('\n');
    }

    for (size_t i = 0; i < y_size; i++) {
        free(matrix[i]);
    }

    free(matrix);

    return 0;
}