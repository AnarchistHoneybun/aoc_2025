#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 1000
#define MAX_NAME_LEN 20
#define MAX_OUTPUTS 50

typedef struct
{
    char name[MAX_NAME_LEN];
    char outputs[MAX_OUTPUTS][MAX_NAME_LEN];
    int output_count;
} Device;

typedef struct
{
    Device devices[MAX_DEVICES];
    int count;
} DeviceMap;

int find_device(DeviceMap *map, const char *name)
{
    for (int i = 0; i < map->count; i++)
    {
        if (strcmp(map->devices[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int count_paths_dfs(DeviceMap *map, const char *current, const char *target, char visited[MAX_DEVICES][MAX_NAME_LEN],
                    int visited_count)
{
    if (strcmp(current, target) == 0)
    {
        return 1;
    }

    int device_idx = find_device(map, current);
    if (device_idx == -1)
    {
        return 0;
    }

    strcpy(visited[visited_count], current);
    visited_count++;

    int total_paths = 0;
    Device *device = &map->devices[device_idx];

    for (int i = 0; i < device->output_count; i++)
    {
        const char *next = device->outputs[i];

        int already_visited = 0;
        for (int j = 0; j < visited_count; j++)
        {
            if (strcmp(visited[j], next) == 0)
            {
                already_visited = 1;
                break;
            }
        }

        if (!already_visited)
        {
            total_paths += count_paths_dfs(map, next, target, visited, visited_count);
        }
    }

    return total_paths;
}

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    DeviceMap map = {0};

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        if (strlen(line) == 0)
            continue;

        Device *device = &map.devices[map.count];
        device->output_count = 0;

        char *colon = strchr(line, ':');
        if (colon == NULL)
            continue;

        int name_len = colon - line;
        strncpy(device->name, line, name_len);
        device->name[name_len] = '\0';

        char *ptr = colon + 1;
        while (*ptr != '\0')
        {
            while (*ptr == ' ' || *ptr == '\t')
                ptr++;
            if (*ptr == '\0')
                break;

            char *start = ptr;
            while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
            {
                ptr++;
            }

            int len = ptr - start;
            if (len > 0 && len < MAX_NAME_LEN)
            {
                strncpy(device->outputs[device->output_count], start, len);
                device->outputs[device->output_count][len] = '\0';
                device->output_count++;
            }
        }

        map.count++;
    }

    char visited[MAX_DEVICES][MAX_NAME_LEN] = {0};
    long long result = count_paths_dfs(&map, "you", "out", visited, 0);

    return result;
}

typedef struct
{
    char key[60];
    long long value;
} CacheEntry;

typedef struct
{
    CacheEntry *entries;
    int count;
    int capacity;
} Cache;

Cache *create_cache()
{
    Cache *c = malloc(sizeof(Cache));
    c->capacity = 500000;
    c->entries = malloc(c->capacity * sizeof(CacheEntry));
    c->count = 0;
    return c;
}

void free_cache(Cache *c)
{
    free(c->entries);
    free(c);
}

long long cache_get(Cache *c, const char *key)
{
    for (int i = 0; i < c->count; i++)
    {
        if (strcmp(c->entries[i].key, key) == 0)
        {
            return c->entries[i].value;
        }
    }
    return -1;
}

void cache_put(Cache *c, const char *key, long long value)
{
    for (int i = 0; i < c->count; i++)
    {
        if (strcmp(c->entries[i].key, key) == 0)
        {
            c->entries[i].value = value;
            return;
        }
    }
    if (c->count < c->capacity)
    {
        strcpy(c->entries[c->count].key, key);
        c->entries[c->count].value = value;
        c->count++;
    }
}

long long count_paths_memo(DeviceMap *map, const char *current, int dac, int fft, Cache *cache)
{
    if (strcmp(current, "out") == 0)
    {
        return (dac && fft) ? 1 : 0;
    }

    int new_dac = dac || (strcmp(current, "dac") == 0);
    int new_fft = fft || (strcmp(current, "fft") == 0);

    char key[60];
    snprintf(key, sizeof(key), "%s,%d,%d", current, new_dac, new_fft);
    long long cached = cache_get(cache, key);
    if (cached != -1)
    {
        return cached;
    }

    int device_idx = find_device(map, current);
    if (device_idx == -1)
    {
        cache_put(cache, key, 0);
        return 0;
    }

    long long total = 0;
    Device *device = &map->devices[device_idx];
    for (int i = 0; i < device->output_count; i++)
    {
        total += count_paths_memo(map, device->outputs[i], new_dac, new_fft, cache);
    }

    cache_put(cache, key, total);
    return total;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    DeviceMap map = {0};

    for (int i = 0; i < n; i++)
    {
        char *line = lines[i];
        if (strlen(line) == 0)
            continue;

        Device *device = &map.devices[map.count];
        device->output_count = 0;

        char *colon = strchr(line, ':');
        if (colon == NULL)
            continue;

        int name_len = colon - line;
        strncpy(device->name, line, name_len);
        device->name[name_len] = '\0';

        char *ptr = colon + 1;
        while (*ptr != '\0')
        {
            while (*ptr == ' ' || *ptr == '\t')
                ptr++;
            if (*ptr == '\0')
                break;

            char *start = ptr;
            while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
            {
                ptr++;
            }

            int len = ptr - start;
            if (len > 0 && len < MAX_NAME_LEN)
            {
                strncpy(device->outputs[device->output_count], start, len);
                device->outputs[device->output_count][len] = '\0';
                device->output_count++;
            }
        }

        map.count++;
    }

    Cache *cache = create_cache();
    long long result = count_paths_memo(&map, "svr", 0, 0, cache);
    free_cache(cache);

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
