#include "../lib/io.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **data;
    size_t rows;
    size_t columns;
} Matrix;

Matrix *matrix_init(const char *input) {
    size_t columns = 0;
    for (size_t i = 0; input[i] != '\n'; i++) {
        columns++;
    }

    size_t rows = 1;
    char **data = NULL;

    size_t i = 0;
    while (input[i] != '\0') {
        data = realloc(data, sizeof(char *) * rows);
        data[rows - 1] = malloc(sizeof(char) * columns);

        size_t column = 0;
        while (input[i] != '\n') {
            data[rows - 1][column] = input[i];
            column++;
            i++;
        }

        rows++;
        i++;
    }

    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rows = rows - 1;
    matrix->columns = columns;
    matrix->data = data;
    return matrix;
}

void matrix_free(Matrix **ptr) {
    Matrix *matrix = *ptr;

    for (size_t row = 0; row < matrix->rows; row++) {
        free(matrix->data[row]);
    }
    free(matrix->data);
    free(matrix);

    *ptr = NULL;
}

bool matrix_indixes_in_bounds(Matrix *matrix, size_t row, size_t column) {
    if (row < matrix->rows && column < matrix->columns) {
        return true;
    }
    return false;
}

char matrix_at(Matrix *matrix, size_t row, size_t column) {
    if (!matrix_indixes_in_bounds(matrix, row, column)) {
        return '\0';
    }
    return matrix->data[row][column];
}

char *matrix_get_str(Matrix *matrix, size_t row, size_t column,
                     ptrdiff_t row_bias, ptrdiff_t column_bias, size_t len) {
    char *str = malloc(sizeof(char) * (len + 1));
    str[len] = '\0';

    size_t current_row = row;
    size_t current_column = column;
    for (size_t i = 0; i < len; i++) {
        str[i] = matrix_at(matrix, current_row, current_column);
        current_row += row_bias;
        current_column += column_bias;
    }

    return str;
}

bool matrix_str_in_position(Matrix *matrix, size_t row, size_t column,
                            ptrdiff_t row_bias, ptrdiff_t column_bias,
                            const char *str) {
    size_t len = strlen(str);
    char *str_in_position =
        matrix_get_str(matrix, row, column, row_bias, column_bias, len);

    bool result = strcmp(str, str_in_position) == 0;

    free(str_in_position);

    return result;
}

size_t matrix_count_strs_in_position(Matrix *matrix, size_t row, size_t column,
                                     const char *str) {
    size_t len = strlen(str);
    char **strs = malloc(sizeof(char *) * 8);

    strs[0] = matrix_get_str(matrix, row, column, 0, 1, len);
    strs[1] = matrix_get_str(matrix, row, column, 1, 0, len);
    strs[2] = matrix_get_str(matrix, row, column, 1, 1, len);
    strs[3] = matrix_get_str(matrix, row, column, 1, -1, len);
    strs[4] = matrix_get_str(matrix, row, column, 0, -1, len);
    strs[5] = matrix_get_str(matrix, row, column, -1, 0, len);
    strs[6] = matrix_get_str(matrix, row, column, -1, -1, len);
    strs[7] = matrix_get_str(matrix, row, column, -1, 1, len);

    size_t count = 0;
    for (size_t i = 0; i < 8; i++) {
        if (strcmp(strs[i], str) == 0) {
            count++;
        }
        free(strs[i]);
    }
    free(strs);

    return count;
}

size_t matrix_count_strs(Matrix *matrix, const char *str) {
    char first = str[0];

    size_t count = 0;
    for (size_t row = 0; row < matrix->rows; row++) {
        for (size_t column = 0; column < matrix->columns; column++) {
            if (matrix_at(matrix, row, column) == first) {
                count +=
                    matrix_count_strs_in_position(matrix, row, column, str);
            }
        }
    }
    return count;
}

bool x_mas_in_position(Matrix *matrix, size_t row, size_t column) {
    bool first_mas = matrix_str_in_position(matrix, row, column, 1, 1, "MAS");
    bool first_sam = matrix_str_in_position(matrix, row, column, 1, 1, "SAM");

    if (!first_mas && !first_sam) {
        return false;
    }

    bool second_mas =
        matrix_str_in_position(matrix, row, column + 2, 1, -1, "MAS");
    bool second_sam =
        matrix_str_in_position(matrix, row, column + 2, 1, -1, "SAM");

    if (!second_mas && !second_sam) {
        return false;
    }

    return true;
}

size_t count_x_mas(Matrix *matrix) {
    size_t count = 0;
    for (size_t row = 0; row < matrix->rows; row++) {
        for (size_t column = 0; column < matrix->columns; column++) {
            if (matrix_at(matrix, row, column) == 'S' ||
                matrix_at(matrix, row, column) == 'M') {
                if (x_mas_in_position(matrix, row, column)) {
                    count++;
                }
            }
        }
    }
    return count;
}

size_t part1(const char *content) {
    Matrix *matrix = matrix_init(content);
    size_t count = matrix_count_strs(matrix, "XMAS");
    matrix_free(&matrix);

    return count;
}

size_t part2(const char *content) {
    Matrix *matrix = matrix_init(content);
    size_t count = count_x_mas(matrix);
    matrix_free(&matrix);

    return count;
}

int main(void) {
    char *content = io_readfile("./inputs/04_day_four.txt");

    printf("answer (part1) = %lu\n", part1(content));
    printf("answer (part2) = %lu\n", part2(content));

    free(content);

    return 0;
}
