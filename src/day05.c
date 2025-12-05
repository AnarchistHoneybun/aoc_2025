#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    int blank_line = -1;
    for (int i = 0; i < n; i++)
    {
        if (lines[i][0] == '\0' || lines[i][0] == '\n' || lines[i][0] == '\r')
        {
            blank_line = i;
            break;
        }
    }

    if (blank_line == -1)
    {
        return 0;
    }

    int num_ranges = blank_line;
    long long *range_starts = malloc(num_ranges * sizeof(long long));
    long long *range_ends = malloc(num_ranges * sizeof(long long));

    for (int i = 0; i < num_ranges; i++)
    {
        char *s = lines[i];
        range_starts[i] = parse_number(&s);
        s++; // skip the -
        range_ends[i] = parse_number(&s);
    }

    for (int i = blank_line + 1; i < n; i++)
    {
        char *s = lines[i];
        long long ingredient_id = parse_number(&s);

        int is_fresh = 0;
        for (int j = 0; j < num_ranges; j++)
        {
            if (ingredient_id >= range_starts[j] && ingredient_id <= range_ends[j])
            {
                is_fresh = 1;
                break;
            }
        }

        if (is_fresh)
        {
            result++;
        }
    }

    free(range_starts);
    free(range_ends);

    return result;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    int blank_line = -1;
    for (int i = 0; i < n; i++)
    {
        if (lines[i][0] == '\0' || lines[i][0] == '\n' || lines[i][0] == '\r')
        {
            blank_line = i;
            break;
        }
    }

    if (blank_line == -1)
    {
        blank_line = n;
    }

    int num_ranges = blank_line;
    long long *range_starts = malloc(num_ranges * sizeof(long long));
    long long *range_ends = malloc(num_ranges * sizeof(long long));

    for (int i = 0; i < num_ranges; i++)
    {
        char *s = lines[i];
        range_starts[i] = parse_number(&s);
        s++;
        range_ends[i] = parse_number(&s);
    }

    for (int i = 0; i < num_ranges - 1; i++)
    {
        for (int j = 0; j < num_ranges - i - 1; j++)
        {
            if (range_starts[j] > range_starts[j + 1])
            {
                long long temp = range_starts[j];
                range_starts[j] = range_starts[j + 1];
                range_starts[j + 1] = temp;

                temp = range_ends[j];
                range_ends[j] = range_ends[j + 1];
                range_ends[j + 1] = temp;
            }
        }
    }

    long long current_start = range_starts[0];
    long long current_end = range_ends[0];

    for (int i = 1; i < num_ranges; i++)
    {
        if (range_starts[i] <= current_end + 1)
        {
            if (range_ends[i] > current_end)
            {
                current_end = range_ends[i];
            }
        }
        else
        {
            result += (current_end - current_start + 1);

            current_start = range_starts[i];
            current_end = range_ends[i];
        }
    }

    result += (current_end - current_start + 1);

    free(range_starts);
    free(range_ends);

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read entire input file into memory
    // n = number of lines, m = length of longest line
    int n, m;
    char **lines = read_input(argv[1], &n, &m);
    if (lines == NULL)
    {
        return EXIT_FAILURE;
    }

    // Time part 1
    long long start = get_time_ns();
    long long result1 = part1(lines, n, m);
    long long end = get_time_ns();
    printf("Part 1: %lld", result1);
    print_time(end - start);
    printf("\n");

    // Time part 2
    start = get_time_ns();
    long long result2 = part2(lines, n, m);
    end = get_time_ns();
    printf("Part 2: %lld", result2);
    print_time(end - start);
    printf("\n");

    free_input(lines);

    return EXIT_SUCCESS;
}
