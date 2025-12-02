#define _POSIX_C_SOURCE 200809L

#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Get current time in nanoseconds
static inline long long get_time_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

// Format time nicely (ns, µs, ms, or s)
static inline void print_time(long long ns)
{
    if (ns < 1000)
    {
        printf(" (%.0f ns)", (double)ns);
    }
    else if (ns < 1000000)
    {
        printf(" (%.2f µs)", ns / 1000.0);
    }
    else if (ns < 1000000000)
    {
        printf(" (%.2f ms)", ns / 1000000.0);
    }
    else
    {
        printf(" (%.2f s)", ns / 1000000000.0);
    }
}

// Based on Frans Faase's approach: https://github.com/FransFaase/AdventOfCode2024/blob/main/Std.md
// Reads entire file into memory, handles any line length
// Returns array of line pointers, caller must free with free_input()
static inline char **read_input(const char *filename, int *n_lines, int *max_line_len)
{
    *n_lines = 0;
    *max_line_len = 0;

    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    int fh = fileno(f);
    size_t length = lseek(fh, 0L, SEEK_END);
    lseek(fh, 0L, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (data == NULL)
    {
        perror("Memory allocation failed");
        fclose(f);
        return NULL;
    }

    length = read(fh, data, length);
    fclose(f);

    for (size_t i = 0; i < length; i++)
    {
        if (data[i] == '\n')
        {
            (*n_lines)++;
        }
    }

    char **lines = (char **)malloc((*n_lines) * sizeof(char *));
    if (lines == NULL)
    {
        perror("Memory allocation failed");
        free(data);
        return NULL;
    }

    char *s = data;
    for (int i = 0; i < *n_lines; i++)
    {
        lines[i] = s;

        int line_len = 0;
        while (*s != '\n')
        {
            if (*s == '\r')
            {
                *s = '\0';
            }
            else
            {
                line_len++;
            }
            s++;
        }

        if (line_len > *max_line_len)
        {
            *max_line_len = line_len;
        }

        *s++ = '\0';
    }

    return lines;
}

static inline void free_input(char **lines)
{
    if (lines != NULL)
    {
        if (lines[0] != NULL)
        {
            free(lines[0]);
        }
        free(lines);
    }
}

// Parses number from string, advances pointer past it
static inline long long parse_number(char **s)
{
    int sign = 1;
    if (**s == '-')
    {
        sign = -1;
        (*s)++;
    }

    long long value = 0;
    while (**s >= '0' && **s <= '9')
    {
        value = 10 * value + (**s - '0');
        (*s)++;
    }

    return sign * value;
}

static inline int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

#endif // AOC_UTILS_H
