#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    int p = 50;
    int zeros = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        char direction = line[0];
        int amt = atoi(line + 1);

        if (direction == 'R')
        {
            p = (p + amt) % 100;
        }
        else if (direction == 'L')
        {
            p = (p - amt) % 100;
            if (p < 0)
                p += 100;
        }

        if (p == 0)
        {
            zeros++;
        }
    }

    return zeros;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    int p = 50;
    int zeros = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        char direction = line[0];
        int amt = atoi(line + 1);

        if (direction == 'R')
        {
            int dist = (100 - p) % 100;
            if (dist == 0)
                dist = 100;

            if (amt >= dist)
            {
                zeros += 1 + (amt - dist) / 100;
            }
            p = (p + amt) % 100;
        }
        else if (direction == 'L')
        {
            int dist = (p != 0) ? p : 100;

            if (amt >= dist)
            {
                zeros += 1 + (amt - dist) / 100;
            }
            p = (p - amt) % 100;
            if (p < 0)
                p += 100;
        }
    }

    return zeros;
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