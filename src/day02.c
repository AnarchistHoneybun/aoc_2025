#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// split in half, check if h1 == h2
// C23: Use bool instead of int for boolean returns
[[nodiscard]] static bool is_invalid(long long num)
{
    char str[32];
    sprintf(str, "%lld", num);
    size_t len = strlen(str);

    // odd length
    if (len % 2 != 0)
    {
        return false;
    }

    size_t half = len / 2;

    return strncmp(str, str + half, half) == 0;
}

// check all pattern lengths, 1..len/2
// C23: [[nodiscard]] attribute warns if return value is ignored
[[nodiscard]] static bool is_invalid_repeated(long long num)
{
    char str[32];
    sprintf(str, "%lld", num);
    size_t len = strlen(str);

    for (size_t pattern_len = 1; pattern_len <= len / 2; pattern_len++)
    {
        if (len % pattern_len != 0)
        {
            continue;
        }

        bool is_match = true;
        for (size_t i = pattern_len; i < len; i++)
        {
            if (str[i] != str[i % pattern_len])
            {
                is_match = false;
                break;
            }
        }

        if (is_match)
        {
            return true;
        }
    }

    return false;
}

void part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        char *ptr = line;

        while (*ptr)
        {
            while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
                ptr++;
            if (!*ptr)
                break;

            long long start = parse_number(&ptr);

            if (*ptr == '-')
                ptr++;

            long long end = parse_number(&ptr);

            for (long long num = start; num <= end; num++)
            {
                if (is_invalid(num))
                {
                    result += num;
                }
            }

            if (*ptr == ',')
                ptr++;
        }
    }

    printf("Part 1: %lld\n", result);
}

void part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        char *ptr = line;

        while (*ptr)
        {
            while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
                ptr++;
            if (!*ptr)
                break;

            long long start = parse_number(&ptr);

            if (*ptr == '-')
                ptr++;

            long long end = parse_number(&ptr);

            for (long long num = start; num <= end; num++)
            {
                if (is_invalid_repeated(num))
                {
                    result += num;
                }
            }

            if (*ptr == ',')
                ptr++;
        }
    }

    printf("Part 2: %lld\n", result);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE; // C23 macro
    }

    // Read entire input file into memory
    // n = number of lines, m = length of longest line
    int n, m;
    char **lines = read_input(argv[1], &n, &m);
    // C23: nullptr instead of NULL
    if (lines == nullptr)
    {
        return EXIT_FAILURE;
    }

    part1(lines, n, m);
    part2(lines, n, m);

    free_input(lines);

    return EXIT_SUCCESS;
}