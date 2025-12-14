#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    int shape_sizes[100];
    int num_shapes = 0;
    int i = 0;

    while (i < n)
    {
        char *line = lines[i];

        if (strchr(line, ':') != NULL && strchr(line, 'x') == NULL)
        {
            i++;

            int cell_count = 0;
            while (i < n && lines[i][0] != '\0' && strchr(lines[i], ':') == NULL)
            {
                for (int j = 0; lines[i][j] != '\0'; j++)
                {
                    if (lines[i][j] == '#')
                    {
                        cell_count++;
                    }
                }
                i++;
            }

            shape_sizes[num_shapes++] = cell_count;
        }
        else if (strchr(line, 'x') != NULL && strchr(line, ':') != NULL)
        {
            int width, height;
            sscanf(line, "%dx%d:", &width, &height);

            long long grid_area = (long long)width * height;

            long long cells_needed = 0;
            char *ptr = strchr(line, ':') + 1;

            for (int s = 0; s < num_shapes && *ptr; s++)
            {
                while (*ptr == ' ')
                    ptr++;

                int count = atoi(ptr);
                cells_needed += (long long)count * shape_sizes[s];

                while (*ptr && *ptr != ' ')
                    ptr++;
            }

            if (cells_needed <= grid_area)
            {
                result++;
            }

            i++;
        }
        else
        {
            i++;
        }
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

    free_input(lines);

    return EXIT_SUCCESS;
}
