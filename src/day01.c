#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1(char **lines, int n, int m)
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

    printf("Part 1: %d\n", zeros);
}

void part2(char **lines, int n, int m)
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

    printf("Part 2: %d\n", zeros);
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