#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        int line_len = strlen(line);

        for (int j = 0; j < line_len; j++)
        {
            if (line[j] == '@')
            {
                int adjacent_count = 0;

                for (int d = 0; d < 8; d++)
                {
                    int ni = i + dx[d];
                    int nj = j + dy[d];

                    if (ni >= 0 && ni < n && nj >= 0 && nj < strlen(lines[ni]))
                    {
                        if (lines[ni][nj] == '@')
                        {
                            adjacent_count++;
                        }
                    }
                }

                // forklist can get to it :)
                if (adjacent_count < 4)
                {
                    result++;
                }
            }
        }
    }

    return result;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    int max_len = 0;
    for (int i = 0; i < n; i++)
    {
        int len = strlen(lines[i]);
        if (len > max_len)
            max_len = len;
    }

    char **grid = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        grid[i] = malloc((max_len + 1) * sizeof(char));
        strcpy(grid[i], lines[i]);
    }

    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

    int removed = 1;
    while (removed > 0)
    {
        removed = 0;

        int min_neighbors = 8;
        int min_i = -1, min_j = -1;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < strlen(grid[i]); j++)
            {
                if (grid[i][j] == '@')
                {
                    int adjacent_count = 0;

                    for (int d = 0; d < 8; d++)
                    {
                        int ni = i + dx[d];
                        int nj = j + dy[d];

                        if (ni >= 0 && ni < n && nj >= 0 && nj < strlen(grid[ni]))
                        {
                            if (grid[ni][nj] == '@')
                            {
                                adjacent_count++;
                            }
                        }
                    }

                    if (adjacent_count < 4 && adjacent_count < min_neighbors)
                    {
                        min_neighbors = adjacent_count;
                        min_i = i;
                        min_j = j;
                    }
                }
            }
        }

        if (min_i != -1)
        {
            grid[min_i][min_j] = '.';
            result++;
            removed = 1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        free(grid[i]);
    }
    free(grid);

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
