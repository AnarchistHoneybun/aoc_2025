#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long total = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];

        char *start = strchr(line, '[');
        char *end = strchr(line, ']');
        int num_lights = end - start - 1;

        int target[32] = {0};
        for (int j = 0; j < num_lights; j++)
        {
            target[j] = (start[j + 1] == '#') ? 1 : 0;
        }

        int num_buttons = 0;
        int buttons[32][32] = {0};

        char *p = end + 1;
        while (*p)
        {
            if (*p == '(')
            {
                p++;
                while (*p && *p != ')')
                {
                    if (*p >= '0' && *p <= '9')
                    {
                        int light_idx = 0;
                        while (*p >= '0' && *p <= '9')
                        {
                            light_idx = light_idx * 10 + (*p - '0');
                            p++;
                        }
                        buttons[num_buttons][light_idx] = 1;
                    }
                    else
                    {
                        p++;
                    }
                }
                if (*p == ')')
                    p++;
                num_buttons++;
            }
            else if (*p == '{')
            {
                break;
            }
            else
            {
                p++;
            }
        }

        int matrix[32][33];
        for (int light = 0; light < num_lights; light++)
        {
            for (int button = 0; button < num_buttons; button++)
            {
                matrix[light][button] = buttons[button][light];
            }
            matrix[light][num_buttons] = target[light];
        }

        int pivot_col[32];
        for (int i = 0; i < 32; i++)
            pivot_col[i] = -1;

        int row = 0;
        for (int col = 0; col < num_buttons && row < num_lights; col++)
        {
            int pivot = -1;
            for (int r = row; r < num_lights; r++)
            {
                if (matrix[r][col] == 1)
                {
                    pivot = r;
                    break;
                }
            }

            if (pivot == -1)
                continue;

            if (pivot != row)
            {
                for (int c = 0; c <= num_buttons; c++)
                {
                    int tmp = matrix[row][c];
                    matrix[row][c] = matrix[pivot][c];
                    matrix[pivot][c] = tmp;
                }
            }

            pivot_col[row] = col;

            for (int r = 0; r < num_lights; r++)
            {
                if (r != row && matrix[r][col] == 1)
                {
                    for (int c = 0; c <= num_buttons; c++)
                    {
                        matrix[r][c] ^= matrix[row][c];
                    }
                }
            }

            row++;
        }

        int free_vars[32];
        int num_free = 0;
        for (int col = 0; col < num_buttons; col++)
        {
            int is_pivot = 0;
            for (int r = 0; r < row; r++)
            {
                if (pivot_col[r] == col)
                {
                    is_pivot = 1;
                    break;
                }
            }
            if (!is_pivot)
            {
                free_vars[num_free++] = col;
            }
        }

        int min_presses = num_buttons + 1;

        for (int mask = 0; mask < (1 << num_free); mask++)
        {
            int solution[32] = {0};

            for (int j = 0; j < num_free; j++)
            {
                solution[free_vars[j]] = (mask >> j) & 1;
            }

            for (int r = row - 1; r >= 0; r--)
            {
                int col = pivot_col[r];
                if (col == -1)
                    continue;

                int val = matrix[r][num_buttons];
                for (int c = col + 1; c < num_buttons; c++)
                {
                    val ^= (matrix[r][c] * solution[c]);
                }
                solution[col] = val;
            }

            int presses = 0;
            for (int j = 0; j < num_buttons; j++)
            {
                presses += solution[j];
            }

            if (presses < min_presses)
            {
                min_presses = presses;
            }
        }

        total += min_presses;
    }

    return total;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    long long total = 0;

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];

        char *p = strchr(line, '{');
        if (!p)
            continue;
        p++;

        int targets[32];
        int num_counters = 0;
        while (*p && *p != '}')
        {
            if (*p >= '0' && *p <= '9')
            {
                int val = 0;
                while (*p >= '0' && *p <= '9')
                {
                    val = val * 10 + (*p - '0');
                    p++;
                }
                targets[num_counters++] = val;
            }
            else
            {
                p++;
            }
        }

        p = strchr(line, ']');
        if (!p)
            continue;
        p++;

        int num_buttons = 0;
        int buttons[32][32] = {0};

        while (*p && *p != '{')
        {
            if (*p == '(')
            {
                p++;
                while (*p && *p != ')')
                {
                    if (*p >= '0' && *p <= '9')
                    {
                        int counter_idx = 0;
                        while (*p >= '0' && *p <= '9')
                        {
                            counter_idx = counter_idx * 10 + (*p - '0');
                            p++;
                        }
                        buttons[num_buttons][counter_idx] = 1;
                    }
                    else
                    {
                        p++;
                    }
                }
                if (*p == ')')
                    p++;
                num_buttons++;
            }
            else
            {
                p++;
            }
        }

        int matrix[32][33];
        for (int counter = 0; counter < num_counters; counter++)
        {
            for (int button = 0; button < num_buttons; button++)
            {
                matrix[counter][button] = buttons[button][counter];
            }
            matrix[counter][num_buttons] = targets[counter];
        }

        int pivot_col[32];
        for (int j = 0; j < 32; j++)
            pivot_col[j] = -1;

        int row = 0;
        for (int col = 0; col < num_buttons && row < num_counters; col++)
        {
            int pivot = -1;
            for (int r = row; r < num_counters; r++)
            {
                if (matrix[r][col] != 0)
                {
                    pivot = r;
                    break;
                }
            }

            if (pivot == -1)
                continue;

            if (pivot != row)
            {
                for (int c = 0; c <= num_buttons; c++)
                {
                    int tmp = matrix[row][c];
                    matrix[row][c] = matrix[pivot][c];
                    matrix[pivot][c] = tmp;
                }
            }

            pivot_col[row] = col;

            for (int r = row + 1; r < num_counters; r++)
            {
                if (matrix[r][col] != 0)
                {
                    int a = matrix[row][col];
                    int b = matrix[r][col];
                    for (int c = 0; c <= num_buttons; c++)
                    {
                        matrix[r][c] = matrix[r][c] * a - matrix[row][c] * b;
                    }
                }
            }

            row++;
        }

        int free_vars[32];
        int num_free = 0;
        for (int col = 0; col < num_buttons; col++)
        {
            int is_pivot = 0;
            for (int r = 0; r < row; r++)
            {
                if (pivot_col[r] == col)
                {
                    is_pivot = 1;
                    break;
                }
            }
            if (!is_pivot)
            {
                free_vars[num_free++] = col;
            }
        }

        long long min_presses = 1000000;
        int max_target = 0;
        for (int j = 0; j < num_counters; j++)
        {
            if (targets[j] > max_target)
                max_target = targets[j];
        }

        int search_limit = (max_target + 1);
        if (num_free > 3)
            search_limit = (max_target / num_free + 2);

        int free_values[32];
        for (int j = 0; j < num_free; j++)
            free_values[j] = 0;

        int done = 0;
        while (!done)
        {
            long long solution[32] = {0};

            for (int j = 0; j < num_free; j++)
            {
                solution[free_vars[j]] = free_values[j];
            }

            int valid = 1;
            for (int r = row - 1; r >= 0 && valid; r--)
            {
                int col = pivot_col[r];
                if (col == -1)
                    continue;

                long long val = matrix[r][num_buttons];
                for (int c = col + 1; c < num_buttons; c++)
                {
                    val -= (long long)matrix[r][c] * solution[c];
                }

                if (matrix[r][col] == 0 || val % matrix[r][col] != 0)
                {
                    valid = 0;
                }
                else
                {
                    solution[col] = val / matrix[r][col];
                    if (solution[col] < 0)
                        valid = 0;
                }
            }

            if (valid)
            {
                long long presses = 0;
                for (int j = 0; j < num_buttons; j++)
                {
                    presses += solution[j];
                }
                if (presses < min_presses)
                {
                    min_presses = presses;
                }
            }

            int pos = 0;
            while (pos < num_free)
            {
                free_values[pos]++;
                if (free_values[pos] < search_limit)
                    break;
                free_values[pos] = 0;
                pos++;
            }
            if (pos >= num_free)
                done = 1;
        }

        total += min_presses;
    }

    return total;
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
