#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(char **grid, int rows, int cols)
{
    int splitCount = 0;

    int startCol = -1;
    for (int c = 0; c < cols; c++)
    {
        if (grid[0][c] == 'S')
        {
            startCol = c;
            break;
        }
    }

    int *activeColumns = calloc(cols, sizeof(int));
    activeColumns[startCol] = 1;

    for (int row = 1; row < rows; row++)
    {
        int *newActiveColumns = calloc(cols, sizeof(int));

        for (int col = 0; col < cols; col++)
        {
            if (activeColumns[col])
            {
                if (grid[row][col] == '^')
                {
                    splitCount++;
                    if (col - 1 >= 0)
                    {
                        newActiveColumns[col - 1] = 1;
                    }
                    if (col + 1 < cols)
                    {
                        newActiveColumns[col + 1] = 1;
                    }
                }
                else
                {
                    newActiveColumns[col] = 1;
                }
            }
        }

        free(activeColumns);
        activeColumns = newActiveColumns;
    }

    free(activeColumns);
    return splitCount;
}

typedef struct
{
    int col;
    int row;
} CacheKey;

typedef struct CacheNode
{
    CacheKey key;
    long long value;
    struct CacheNode *next;
} CacheNode;

#define CACHE_SIZE 10007

typedef struct
{
    CacheNode *table[CACHE_SIZE];
} Cache;

unsigned int hashKey(int col, int row)
{
    unsigned int hash = 17;
    hash = hash * 31 + col;
    hash = hash * 31 + row;
    return hash % CACHE_SIZE;
}

Cache *createCache()
{
    Cache *cache = malloc(sizeof(Cache));
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        cache->table[i] = NULL;
    }
    return cache;
}

long long getCached(Cache *cache, int col, int row)
{
    unsigned int idx = hashKey(col, row);
    CacheNode *node = cache->table[idx];
    while (node != NULL)
    {
        if (node->key.col == col && node->key.row == row)
        {
            return node->value;
        }
        node = node->next;
    }
    return -1; // nound
}

void putCache(Cache *cache, int col, int row, long long value)
{
    unsigned int idx = hashKey(col, row);
    CacheNode *node = malloc(sizeof(CacheNode));
    node->key.col = col;
    node->key.row = row;
    node->value = value;
    node->next = cache->table[idx];
    cache->table[idx] = node;
}

void freeCache(Cache *cache)
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        CacheNode *node = cache->table[i];
        while (node != NULL)
        {
            CacheNode *next = node->next;
            free(node);
            node = next;
        }
    }
    free(cache);
}

long long countTimelines(int col, int row, int maxRow, int **splitters, int numSplitters, Cache *cache)
{
    if (row == maxRow)
    {
        return 1;
    }

    long long cached = getCached(cache, col, row);
    if (cached != -1)
    {
        return cached;
    }

    long long result;

    int hasSplitter = 0;
    for (int i = 0; i < numSplitters; i++)
    {
        if (splitters[i][0] == row && splitters[i][1] == col)
        {
            hasSplitter = 1;
            break;
        }
    }

    if (hasSplitter)
    {
        long long leftTimelines = countTimelines(col - 1, row + 1, maxRow, splitters, numSplitters, cache);
        long long rightTimelines = countTimelines(col + 1, row + 1, maxRow, splitters, numSplitters, cache);
        result = leftTimelines + rightTimelines;
    }
    else
    {
        result = countTimelines(col, row + 1, maxRow, splitters, numSplitters, cache);
    }

    putCache(cache, col, row, result);

    return result;
}

long long part2(char **grid, int rows, int cols)
{
    int startCol = -1;
    for (int c = 0; c < cols; c++)
    {
        if (grid[0][c] == 'S')
        {
            startCol = c;
            break;
        }
    }

    int **splitters = malloc(sizeof(int *) * rows * cols);
    int numSplitters = 0;

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == '^')
            {
                splitters[numSplitters] = malloc(sizeof(int) * 2);
                splitters[numSplitters][0] = r;
                splitters[numSplitters][1] = c;
                numSplitters++;
            }
        }
    }

    Cache *cache = createCache();

    long long result = countTimelines(startCol, 0, rows - 1, splitters, numSplitters, cache);

    freeCache(cache);
    for (int i = 0; i < numSplitters; i++)
    {
        free(splitters[i]);
    }
    free(splitters);

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
