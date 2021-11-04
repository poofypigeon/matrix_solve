#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

#include "fraction.h"
#include "input.h"

void reduce_matrix (fraction **matrix, size_t x_size, size_t y_size) {
    size_t rank = 0;

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
