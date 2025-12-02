#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    // Access input: lines[i] contains line i (zero-indexed)
    // Example usage:
    //   char *first_line = lines[0];
    //   for (int i = 0; i < n; i++) { ... }
    //
    // Parse numbers from a line:
    //   char *s = lines[i];
    //   long long num = parse_number(&s);  // advances s past the number

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];

        // TODO: Implement part 1 solution
    }

    return result;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];

        // TODO: Implement part 2 solution
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
