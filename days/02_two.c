#include "../lib/io.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define INCREASING 1
#define DECREASING 2

const char *collect_number(const char *digits, int32_t *number) {
    int i = 0;
    while (isdigit(digits[i])) {
        i++;
    }
    *number = atoi(digits);

    if (digits[i] != ' ') {
        return &digits[i];
    }
    return &digits[i + 1];
}

int32_t *line_to_numbers(const char *line, size_t *len, uint8_t *bias) {
    *len = 0;
    int32_t *numbers = realloc(NULL, sizeof(int32_t) * *len);

    int32_t bias_count = 0;
    int32_t temp;
    const char *next = line;
    do {
        next = collect_number(next, &temp);
        (*len)++;
        numbers = realloc(numbers, sizeof(int32_t) * *len);
        numbers[*len - 1] = temp;
        if (*len > 1 && (numbers[*len - 2] < numbers[*len - 1])) {
            bias_count++;
        } else if (*len > 1 && (numbers[*len - 2] > numbers[*len - 1])) {
            bias_count--;
        }
    } while (next[0] != '\n' && next[0] != '\0');

    *bias = bias_count > 0 ? INCREASING : DECREASING;

    return numbers;
}

ptrdiff_t get_unsafe(int32_t *numbers, size_t len, uint8_t bias) {
    size_t i = 1;
    do {
        int32_t first = numbers[i - 1];
        int32_t second = numbers[i];

        if (first == second) {
            return i;
        }

        int32_t diff;
        if (bias == INCREASING) {
            diff = second - first;
        } else {
            diff = first - second;
        }

        if (diff < 1 || diff > 3) {
            return i;
        }
        i++;
    } while (i < len);

    return -1;
}

int32_t part1(const char *content) {
    int32_t safes_count = 0;
    size_t i = 0;
    while (isdigit(content[i])) {
        size_t len;
        uint8_t bias;
        int32_t *numbers = line_to_numbers(&content[i], &len, &bias);

        if (get_unsafe(numbers, len, bias) == -1) {
            safes_count++;
        }
        free(numbers);

        while (content[i] != '\n') {
            i++;
        }
        i++;
    }

    return safes_count;
}

int32_t remove_index(int32_t *arr, size_t len, size_t index) {
    int value = arr[index];

    for (size_t i = index; i < len - 1; i++) {
        arr[i] = arr[i + 1];
    }

    return value;
}

void add_index(int32_t *arr, size_t len, size_t index, int32_t value) {
    for (size_t i = len; i > index; i--) {
        arr[i] = arr[i - 1];
    }
    arr[index] = value;
}

int32_t part2(const char *content) {
    int32_t safes_count = 0;
    size_t i = 0;
    while (isdigit(content[i])) {
        size_t len;
        uint8_t bias;
        int32_t *numbers = line_to_numbers(&content[i], &len, &bias);

        ptrdiff_t unsafe = get_unsafe(numbers, len, bias);
        if (unsafe == -1) {
            safes_count++;
        } else {
            int32_t removed = remove_index(numbers, len, unsafe);
            ptrdiff_t retry = get_unsafe(numbers, len - 1, bias);
            add_index(numbers, len - 1, unsafe, removed);

            if (retry == -1) {
                safes_count++;
            } else {
                removed = remove_index(numbers, len, unsafe - 1);
                retry = get_unsafe(numbers, len - 1, bias);
                add_index(numbers, len - 1, unsafe - 1, removed);

                if (retry == -1) {
                    safes_count++;
                }
            }
        }

        while (content[i] != '\n') {
            i++;
        }
        i++;
    }

    return safes_count;
}

int main(void) {
    char *content = io_readfile("./inputs/02_day_two.txt");

    printf("answer (part1) = %d\n", part1(content));
    printf("answer (part2) = %d\n", part2(content));

    free(content);

    return 0;
}
