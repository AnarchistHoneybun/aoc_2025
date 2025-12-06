#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    if (n == 0)
        return 0;

    int col = 0;
    while (col < m)
    {
        // Skip empty columns (problem separators)
        int is_empty = 1;
        for (int row = 0; row < n; row++)
        {
            if (col < (int)strlen(lines[row]) && lines[row][col] != ' ')
            {
                is_empty = 0;
                break;
            }
        }

        if (is_empty)
        {
            col++;
            continue;
        }

        int problem_start = col;
        int problem_end = col;

        while (problem_end < m)
        {
            int col_empty = 1;
            for (int row = 0; row < n; row++)
            {
                if (problem_end < (int)strlen(lines[row]) && lines[row][problem_end] != ' ')
                {
                    col_empty = 0;
                    break;
                }
            }
            if (col_empty)
                break;
            problem_end++;
        }

        char op = '\0';
        for (int c = problem_start; c < problem_end; c++)
        {
            if (c < (int)strlen(lines[n - 1]))
            {
                char ch = lines[n - 1][c];
                if (ch == '+' || ch == '*')
                {
                    op = ch;
                    break;
                }
            }
        }

        long long problem_result = (op == '+') ? 0 : 1;

        for (int row = 0; row < n - 1; row++)
        {
            char num_str[100] = {0};
            int num_idx = 0;

            for (int c = problem_start; c < problem_end; c++)
            {
                if (c < (int)strlen(lines[row]))
                {
                    char ch = lines[row][c];
                    if (ch >= '0' && ch <= '9')
                    {
                        num_str[num_idx++] = ch;
                    }
                }
            }

            if (num_idx > 0)
            {
                num_str[num_idx] = '\0';
                long long num = atoll(num_str);

                if (op == '+')
                {
                    problem_result += num;
                }
                else if (op == '*')
                {
                    problem_result *= num;
                }
            }
        }

        result += problem_result;
        col = problem_end;
    }

    return result;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    if (n == 0)
        return 0;

    int col = 0;
    while (col < m)
    {
        int is_empty = 1;
        for (int row = 0; row < n; row++)
        {
            if (col < (int)strlen(lines[row]) && lines[row][col] != ' ')
            {
                is_empty = 0;
                break;
            }
        }

        if (is_empty)
        {
            col++;
            continue;
        }

        int problem_start = col;
        int problem_end = col;

        while (problem_end < m)
        {
            int col_empty = 1;
            for (int row = 0; row < n; row++)
            {
                if (problem_end < (int)strlen(lines[row]) && lines[row][problem_end] != ' ')
                {
                    col_empty = 0;
                    break;
                }
            }
            if (col_empty)
                break;
            problem_end++;
        }

        char op = '\0';
        for (int c = problem_start; c < problem_end; c++)
        {
            if (c < (int)strlen(lines[n - 1]))
            {
                char ch = lines[n - 1][c];
                if (ch == '+' || ch == '*')
                {
                    op = ch;
                    break;
                }
            }
        }

        long long problem_result = (op == '+') ? 0 : 1;

        for (int c = problem_end - 1; c >= problem_start; c--)
        {
            char num_str[100] = {0};
            int num_idx = 0;

            for (int row = 0; row < n - 1; row++)
            {
                if (c < (int)strlen(lines[row]))
                {
                    char ch = lines[row][c];
                    if (ch >= '0' && ch <= '9')
                    {
                        num_str[num_idx++] = ch;
                    }
                }
            }

            if (num_idx > 0)
            {
                num_str[num_idx] = '\0';
                long long num = atoll(num_str);

                if (op == '+')
                {
                    problem_result += num;
                }
                else if (op == '*')
                {
                    problem_result *= num;
                }
            }
        }

        result += problem_result;
        col = problem_end;
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
