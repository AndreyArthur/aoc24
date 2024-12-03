#include "../lib/io.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t *collect_mul(const char *memory) {
    bool is_valid_prefix = memcmp(memory, "mul(", sizeof(char) * 4) == 0;
    if (!is_valid_prefix) {
        return NULL;
    }
    const char *current = &memory[4];

    size_t i = 0;
    if (!isdigit(current[i])) {
        return NULL;
    }
    int32_t first = atoi(&current[i]);

    size_t digits = 1;
    i++;
    while (isdigit(current[i])) {
        digits++;
        i++;

        if (digits > 3) {
            return NULL;
        }
    }

    if (current[i] != ',') {
        return NULL;
    }
    i++;

    if (!isdigit(current[i])) {
        return NULL;
    }
    int32_t second = atoi(&current[i]);

    digits = 1;
    i++;
    while (isdigit(current[i])) {
        digits++;
        i++;

        if (digits > 3) {
            return NULL;
        }
    }

    if (current[i] != ')') {
        return NULL;
    }

    int32_t *values = malloc(sizeof(int32_t) * 2);
    values[0] = first;
    values[1] = second;
    return values;
}

int32_t **get_mul_instructions(const char *memory, size_t *len) {
    *len = 0;
    int32_t **instructions = realloc(NULL, sizeof(int32_t *) * *len);
    size_t i = 0;
    while (memory[i] != '\0') {
        while (memory[i] != 'm' && memory[i] != '\0') {
            i++;
        }

        if (memory[i] == '\0') {
            continue;
        }

        int32_t *instruction = collect_mul(&memory[i]);
        if (instruction == NULL) {
            i++;
            continue;
        }

        (*len)++;
        instructions = realloc(instructions, sizeof(int32_t *) * *len);
        instructions[*len - 1] = instruction;
        i++;
    }
    return instructions;
}

int32_t **get_mul_instructions_with_clauses(const char *memory, size_t *len) {
    *len = 0;
    int32_t **instructions = realloc(NULL, sizeof(int32_t *) * *len);
    size_t i = 0;
    bool is_enabled = true;
    while (memory[i] != '\0') {
        while (memory[i] != 'm' && memory[i] != 'd' && memory[i] != '\0') {
            i++;
        }

        if (memory[i] == '\0') {
            continue;
        }

        if (memory[i] == 'd') {
            bool is_do = memcmp(&memory[i], "do()", sizeof(char) * 4) == 0;
            if (is_do) {
                is_enabled = true;
                i++;
                continue;
            }

            bool is_dont = memcmp(&memory[i], "don't()", sizeof(char) * 7) == 0;
            if (is_dont) {
                is_enabled = false;
                i++;
                continue;
            }

            i++;
            continue;
        }

        if (is_enabled) {
            int32_t *instruction = collect_mul(&memory[i]);
            if (instruction == NULL) {
                i++;
                continue;
            }

            (*len)++;
            instructions = realloc(instructions, sizeof(int32_t *) * *len);
            instructions[*len - 1] = instruction;
        }
        i++;
    }
    return instructions;
}

int64_t sum_instructions(int32_t **instructions, size_t len) {
    int64_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += instructions[i][0] * instructions[i][1];
    }
    return sum;
}

int64_t part1(const char *content) {
    size_t len;
    int32_t **instructions = get_mul_instructions(content, &len);
    int64_t sum = sum_instructions(instructions, len);

    for (size_t i = 0; i < len; i++) {
        free(instructions[i]);
    }
    free(instructions);

    return sum;
}

int64_t part2(const char *content) {
    size_t len;
    int32_t **instructions = get_mul_instructions_with_clauses(content, &len);
    int64_t sum = sum_instructions(instructions, len);

    for (size_t i = 0; i < len; i++) {
        free(instructions[i]);
    }
    free(instructions);

    return sum;
}

int main(void) {
    char *content = io_readfile("./inputs/03_day_three.txt");

    printf("answer (part1) = %ld\n", part1(content));
    printf("answer (part2) = %ld\n", part2(content));

    return 0;
}
