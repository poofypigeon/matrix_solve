#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DOUBLE_MARGIN 1e-10
#define double_equals(d, v) (((d) < ((v) + DOUBLE_MARGIN)) && ((d) > ((v) - DOUBLE_MARGIN)))

void reduce_matrix(double** matrix, size_t x_size, size_t y_size) 
{
    size_t rank = 0;

    for (size_t col = 0; col < x_size; col++) {
        bool leading_entry_found = false;

        // start at first row without a determined leading entry
        for (size_t row = rank; row < y_size; row++) {
            // determine if there is a leading entry 
            if (!double_equals(matrix[row][col], 0.0)) {
                if (row != rank) {
                    double *temp = matrix[row];
                    matrix[row] = matrix[rank];
                    matrix[rank] = temp;
                }

                double leading_value =  matrix[rank][col];
                for (size_t i = col; i < x_size; i++) {
                    matrix[rank][i] /= leading_value;
                }

                leading_entry_found = true;
                break;
            }
        }

        // if the column beyond current rank is not all zeros
        if (leading_entry_found) {
            for (size_t row = 0; row < y_size; row++) {
                if (row != rank && !double_equals(matrix[row][col], 0.0)) {
                    double leading_value = matrix[row][col];
                    for (size_t i = col; i < x_size; i++) {
                        matrix[row][i] -= (matrix[rank][i] * leading_value);
                    }
                }
            }

            rank++;
        }
    }
}

int main () {
    size_t x_size, y_size;

    printf("Rows: ");
    scanf("%zu", &y_size);

    printf("Columns: ");
    scanf("%zu", &x_size);

    double **matrix = calloc(y_size, sizeof(double **));

    for (int i = 0; i < y_size; i++) {
        matrix[i] = calloc(x_size, sizeof(double **));
    }

    printf("Entries:\n");
    for (size_t y = 0; y < y_size; y++) {
        for (size_t x = 0; x < x_size; x++) {
            scanf("%lf", &matrix[y][x]);
        }
    }

    reduce_matrix(matrix, x_size, y_size);

    putchar('\n');

    for (size_t y = 0; y < y_size; y++) {
        for (size_t x = 0; x < x_size; x++) {
            bool rational = false;

            if (double_equals(matrix[y][x] - (long long) matrix[y][x], 0.0)) {
                if ((long long) matrix[y][x] >= 0) putchar(' ');
                printf("%lld ", (long long) matrix[y][x]);
                rational = true;
            }

            else {
                double denominator = 1 / matrix[y][x];
                bool negative = false;
                

                if (denominator < 0) {
                    negative = true;
                    denominator *= -1;
                }

                for (size_t n = 1; n <= 1024; n++) {
                    double d = denominator * n;

                    if (double_equals(d - (long long) d, 0.0)) {
                        putchar(negative ? '-' : ' ');

                        printf("%zu/%lld ", n, (long long) d);
                        rational = true;
                        break;
                    }
                }

            if (!rational) printf("%.2f ", matrix[y][x]);

            }
        }
        putchar('\n');
    }

    for (int i = 0; i < y_size; i++) {
        free(matrix[i]);
    }

    free(matrix);
}