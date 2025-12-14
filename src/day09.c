#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    long long result = 0;

    int *x_coords = malloc(n * sizeof(int));
    int *y_coords = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        char *s = line;

        x_coords[i] = parse_number(&s);

        s++;

        y_coords[i] = parse_number(&s);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            long long width = llabs(x_coords[j] - x_coords[i]) + 1;
            long long height = llabs(y_coords[j] - y_coords[i]) + 1;
            long long area = width * height;

            if (area > result)
            {
                result = area;
            }
        }
    }

    free(x_coords);
    free(y_coords);

    return result;
}

#include <math.h>

static int is_between(double v, double a, double b)
{
    return (v > a && v < b) || (v > b && v < a);
}

static int ranges_overlap(double a1, double a2, double b1, double b2)
{
    double min_a = (a1 < a2) ? a1 : a2;
    double max_a = (a1 > a2) ? a1 : a2;
    double min_b = (b1 < b2) ? b1 : b2;
    double max_b = (b1 > b2) ? b1 : b2;

    return max_a > min_b && max_b > min_a;
}

static int on_segment(double px, double py, int ax, int ay, int bx, int by)
{
    if (ax == bx)
    {
        if (px != ax)
            return 0;
        return (py >= ay && py <= by) || (py >= by && py <= ay);
    }
    else if (ay == by)
    {
        if (py != ay)
            return 0;
        return (px >= ax && px <= bx) || (px >= bx && px <= ax);
    }
    return 0;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    int *x_coords = malloc(n * sizeof(int));
    int *y_coords = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        char *s = line;
        x_coords[i] = parse_number(&s);
        s++;
        y_coords[i] = parse_number(&s);
    }

    long long max_result = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int x1 = x_coords[i];
            int y1 = y_coords[i];
            int x2 = x_coords[j];
            int y2 = y_coords[j];

            int min_x = (x1 < x2) ? x1 : x2;
            int max_x = (x1 > x2) ? x1 : x2;
            int min_y = (y1 < y2) ? y1 : y2;
            int max_y = (y1 > y2) ? y1 : y2;

            double cx = (min_x + max_x) / 2.0;
            double cy = (min_y + max_y) / 2.0;

            int is_inside = 0;
            int on_boundary = 0;

            for (int k = 0; k < n; k++)
            {
                int next_k = (k + 1) % n;
                if (on_segment(cx, cy, x_coords[k], y_coords[k], x_coords[next_k], y_coords[next_k]))
                {
                    on_boundary = 1;
                    break;
                }
            }

            if (on_boundary)
            {
                is_inside = 1;
            }
            else
            {
                int intersections = 0;
                for (int k = 0; k < n; k++)
                {
                    int next_k = (k + 1) % n;
                    int ax = x_coords[k];
                    int ay = y_coords[k];
                    int bx = x_coords[next_k];
                    int by = y_coords[next_k];

                    if ((ay > cy) != (by > cy))
                    {
                        double ix = ax + (double)(bx - ax) * (cy - ay) / (by - ay);
                        if (ix > cx)
                        {
                            intersections++;
                        }
                    }
                }
                if (intersections % 2 == 1)
                    is_inside = 1;
            }

            if (!is_inside)
                continue;

            int intersects_interior = 0;
            for (int k = 0; k < n; k++)
            {
                int next_k = (k + 1) % n;
                int ax = x_coords[k];
                int ay = y_coords[k];
                int bx = x_coords[next_k];
                int by = y_coords[next_k];

                if (ax == bx)
                {
                    if (is_between(ax, min_x, max_x) && ranges_overlap(min_y, max_y, ay, by))
                    {
                        intersects_interior = 1;
                        break;
                    }
                }
                else
                {
                    if (is_between(ay, min_y, max_y) && ranges_overlap(min_x, max_x, ax, bx))
                    {
                        intersects_interior = 1;
                        break;
                    }
                }
            }

            if (intersects_interior)
                continue;

            long long width = (long long)(max_x - min_x) + 1;
            long long height = (long long)(max_y - min_y) + 1;
            long long area = width * height;

            if (area > max_result)
            {
                max_result = area;
            }
        }
    }

    free(x_coords);
    free(y_coords);

    return max_result;
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
