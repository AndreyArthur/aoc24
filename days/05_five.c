#include "../lib/io.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *pa, const void *pb) {
    uint8_t a = *(uint8_t *)pa;
    uint8_t b = *(uint8_t *)pb;
    return a - b;
}

uint8_t **get_ordering(const char *ordering_as_str) {
    uint8_t **ordering = malloc(sizeof(uint8_t *) * 100);
    for (size_t i = 0; i < 100; i++) {
        ordering[i] = malloc(sizeof(uint8_t) * 1);
        ordering[i][0] = 0;
    }

    size_t i = 0;
    while (isdigit(ordering_as_str[i])) {
        uint8_t l, r;
        sscanf(&ordering_as_str[i], "%hhu|%hhu", &l, &r);

        ordering[l] =
            realloc(ordering[l], sizeof(uint8_t) * (ordering[l][0] + 2));
        ordering[l][0]++;
        ordering[l][ordering[l][0]] = r;

        i += 6;
    }

    for (i = 0; i < 100; i++) {
        if (ordering[i][0] != 0) {
            size_t len = ordering[i][0];
            uint8_t *arr = &ordering[i][1];

            qsort(arr, len, sizeof(uint8_t), cmp);
        }
    }

    return ordering;
}

void free_ordering(uint8_t **ordering) {
    for (size_t i = 0; i < 100; i++) {
        free(ordering[i]);
    }
    free(ordering);
}

bool follows_ordering(uint8_t **ordering, uint8_t key, uint8_t value) {
    uint8_t *ptr = bsearch(&value, &ordering[key][1], ordering[key][0],
                           sizeof(uint8_t), cmp);
    return ptr != NULL;
}

bool list_is_ordered(uint8_t **ordering, uint8_t *list, size_t len) {
    for (size_t i = 0; i < len - 1; i++) {
        uint8_t key = list[i];
        for (size_t j = i + 1; j < len; j++) {
            bool is_ok = follows_ordering(ordering, key, list[j]);
            if (!is_ok) {
                return false;
            }
        }
    }
    return true;
}

uint8_t **get_lists(const char *lists_as_str, size_t *len) {
    *len = 0;
    uint8_t **lists = realloc(NULL, sizeof(uint8_t *) * *len);

    size_t i = 0;
    while (isdigit(lists_as_str[i])) {
        (*len)++;
        lists = realloc(lists, sizeof(uint8_t *) * *len);

        lists[*len - 1] = malloc(sizeof(uint8_t) * 1);
        lists[*len - 1][0] = 0;

        uint8_t *list = lists[*len - 1];

        while (isdigit(lists_as_str[i])) {
            uint8_t current = atoi(&lists_as_str[i]);

            list[0]++;
            list = realloc(list, sizeof(uint8_t) * (list[0] + 1));
            list[list[0]] = current;

            while (lists_as_str[i] != ',' && lists_as_str[i] != '\n') {
                i++;
            }
            if (lists_as_str[i] != '\n') {
                i++;
            }
        }
        i++;
    }

    return lists;
}

size_t find_prior(uint8_t **ordering, uint8_t *list, size_t len) {
    bool follows = false;
    size_t index = 0;
    while (follows == false) {
        follows = true;
        for (size_t i = 0; i < len; i++) {
            if (i != index) {
                bool is_ok = follows_ordering(ordering, list[index], list[i]);
                if (!is_ok) {
                    follows = false;
                    index++;
                    break;
                }
            }
        }
    }
    return index;
}

void order_list(uint8_t **ordering, uint8_t *list, size_t len) {
    for (size_t i = 0; i < len; i++) {
        size_t index = find_prior(ordering, &list[i], len - i);
        index += i;
        uint8_t temp = list[index];
        list[index] = list[i];
        list[i] = temp;
    }
}

uint32_t part1(const char *content) {
    uint8_t **ordering = get_ordering(content);

    size_t i = 0;
    while (memcmp(&content[i], "\n\n", sizeof(char) * 2) != 0) {
        i++;
    }
    i += 2;

    size_t len;
    uint8_t **lists = get_lists(&content[i], &len);

    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        if (list_is_ordered(ordering, &lists[i][1], lists[i][0])) {
            size_t middle = lists[i][0] / 2;
            sum += (&lists[i][1])[middle];
        }
    }

    free_ordering(ordering);

    return sum;
}

uint32_t part2(const char *content) {
    uint8_t **ordering = get_ordering(content);

    size_t i = 0;
    while (memcmp(&content[i], "\n\n", sizeof(char) * 2) != 0) {
        i++;
    }
    i += 2;

    size_t len;
    uint8_t **lists = get_lists(&content[i], &len);

    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        if (!list_is_ordered(ordering, &lists[i][1], lists[i][0])) {
            uint8_t *temporary_list =
                malloc(sizeof(uint8_t) * (lists[i][0] + 1));
            memcpy(temporary_list, lists[i],
                   sizeof(uint8_t) * (lists[i][0] + 1));

            order_list(ordering, &temporary_list[1], temporary_list[0]);

            size_t middle = temporary_list[0] / 2;
            sum += (&temporary_list[1])[middle];

            free(temporary_list);
        }
    }

    free_ordering(ordering);

    return sum;
}

int main(void) {
    const char *content = io_readfile("./inputs/05_day_five.txt");

    printf("answer (part1) = %u\n", part1(content));
    printf("answer (part2) = %u\n", part2(content));

    return 0;
}
