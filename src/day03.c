#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        int len = strlen(line);

        // brute force :(
        int max_joltage = 0;

        for (int j = 0; j < len; j++)
        {
            if (line[j] >= '0' && line[j] <= '9')
            {
                for (int k = j + 1; k < len; k++)
                {
                    if (line[k] >= '0' && line[k] <= '9')
                    {
                        // line[j] is tens, line[k] is ones
                        int joltage = (line[j] - '0') * 10 + (line[k] - '0');
                        if (joltage > max_joltage)
                        {
                            max_joltage = joltage;
                        }
                    }
                }
            }
        }

        result += max_joltage;
    }

    return result;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        int len = strlen(line);

        int digit_count = len;

        // greedy approach
        long long max_joltage = 0;
        int start_pos = 0;

        for (int selected = 0; selected < 12; selected++)
        {
            int remaining_needed = 12 - selected - 1;
            int best_digit = -1;
            int best_pos = -1;

            // find largest given enough are left to pick remaining
            for (int j = start_pos; j < digit_count; j++)
            {
                int remaining_after = digit_count - j - 1;
                if (remaining_after >= remaining_needed)
                {
                    int digit = line[j] - '0';
                    if (digit > best_digit)
                    {
                        best_digit = digit;
                        best_pos = j;
                    }
                }
            }

            max_joltage = max_joltage * 10 + best_digit;
            start_pos = best_pos + 1;
        }

        result += max_joltage;
    }

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
