#include "../lib/io.h"
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_lists(const char *raw, int32_t **left, int32_t **right) {
    int32_t l, r;

    *left = NULL;
    *right = NULL;

    size_t len = 0;
    int i = 0;
    while (isdigit(raw[i])) {
        sscanf(&raw[i], "%d %d", &l, &r);
        len++;

        *left = realloc(*left, sizeof(int32_t) * len);
        *right = realloc(*right, sizeof(int32_t) * len);
        (*left)[len - 1] = l;
        (*right)[len - 1] = r;

        while (raw[i] != '\n' && raw[i] != '\0') {
            i++;
        }
        if (raw[i] == '\n') {
            i++;
        }
    }

    return len;
}

int32_t *get_differences(int32_t *left, int32_t *right, size_t len) {
    int32_t *differences = malloc(sizeof(int32_t) * len);

    for (size_t i = 0; i < len; i++) {
        differences[i] = abs(left[i] - right[i]);
    }

    return differences;
}

int32_t *get_similarities(int32_t *left, int32_t *right, size_t len) {
    int32_t *similarities = malloc(sizeof(int32_t) * len);

    // pair && count method
    size_t i = 0, j = 0, recover = j;
    while (i < len) {
        similarities[i] = 0;
        while (left[i] > right[j]) {
            j++;
        }

        while (right[j] == left[i]) {
            similarities[i] += right[j];
            j++;
        }
        i++;
        if (i < len && left[i] == left[i - 1]) {
            j = recover;
        } else {
            recover = j;
        }
    }

    return similarities;
}

int32_t sum(int32_t *arr, size_t len) {
    int32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += arr[i];
    }
    return sum;
}

int cmp(const void *pa, const void *pb) {
    int32_t a = *(int32_t *)pa;
    int32_t b = *(int32_t *)pb;
    return a - b;
}

int32_t part1(const char *content) {
    int32_t *left;
    int32_t *right;
    size_t len = get_lists(content, &left, &right);

    qsort(left, len, sizeof(int32_t), cmp);
    qsort(right, len, sizeof(int32_t), cmp);

    int32_t *differences = get_differences(left, right, len);

    free(left);
    free(right);

    int32_t result = sum(differences, len);

    free(differences);

    return result;
}

int32_t part2(const char *content) {
    int32_t *left;
    int32_t *right;
    size_t len = get_lists(content, &left, &right);

    qsort(left, len, sizeof(int32_t), cmp);
    qsort(right, len, sizeof(int32_t), cmp);

    int32_t *similarities = get_similarities(left, right, len);

    free(left);
    free(right);

    int32_t result = sum(similarities, len);

    free(similarities);

    return result;
}

int main(void) {
    char *content = io_readfile("./inputs/01_day_one.txt");

    printf("answer (part1) = %d\n", part1(content));
    printf("answer (part2) = %d\n", part2(content));

    free(content);

    return 0;
}
