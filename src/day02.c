#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pattern repeated twice
int is_invalid(long long num)
{
    char str[32];
    sprintf(str, "%lld", num);
    int len = strlen(str);

    if (len % 2 != 0)
    {
        return 0;
    }

    int half = len / 2;

    // if first half equals second half
    return strncmp(str, str + half, half) == 0;
}

void part1(char **lines, int n, int m)
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

int is_invalid_repeated(long long num)
{
    char str[32];
    sprintf(str, "%lld", num);
    int len = strlen(str);

    // try all possible pattern lengths from 1 to len/2
    for (int pattern_len = 1; pattern_len <= len / 2; pattern_len++)
    {
        // if the total length is divisible by pattern length
        if (len % pattern_len != 0)
        {
            continue;
        }

        // check if repeating the first pattern_len characters gives us the whole string
        int is_match = 1;
        for (int i = pattern_len; i < len; i++)
        {
            if (str[i] != str[i % pattern_len])
            {
                is_match = 0;
                break;
            }
        }

        if (is_match)
        {
            return 1;
        }
    }

    return 0;
}

void part2(char **lines, int n, int m)
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
        return 1;
    }

    // Read entire input file into memory
    // n = number of lines, m = length of longest line
    int n, m;
    char **lines = read_input(argv[1], &n, &m);
    if (lines == NULL)
    {
        return 1;
    }

    part1(lines, n, m);
    part2(lines, n, m);

    free_input(lines);

    return 0;
}
