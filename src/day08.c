#define _POSIX_C_SOURCE 200809L

#include "aoc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

typedef struct
{
    int x, y, z;
} Point;

typedef struct
{
    long long dist_sq;
    int i, j;
} Edge;

typedef struct
{
    int *parent;
    int *size;
    int comp_count;
    int n;
} UnionFind;

Point *parse_points(char **lines, int n)
{
    Point *points = malloc(n * sizeof(Point));
    for (int i = 0; i < n; i++)
    {
        char *s = lines[i];
        points[i].x = parse_number(&s);
        s++;
        points[i].y = parse_number(&s);
        s++;
        points[i].z = parse_number(&s);
    }
    return points;
}

long long dist_squared(Point *a, Point *b)
{
    long long dx = (long long)(a->x - b->x);
    long long dy = (long long)(a->y - b->y);
    long long dz = (long long)(a->z - b->z);
    return dx * dx + dy * dy + dz * dz;
}

int compare_edges(const void *a, const void *b)
{
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;
    if (ea->dist_sq < eb->dist_sq)
        return -1;
    if (ea->dist_sq > eb->dist_sq)
        return 1;
    return 0;
}

Edge *create_sorted_edges(Point *points, int n, int *edge_count)
{
    int total_edges = (n * (n - 1)) / 2;
    Edge *edges = malloc(total_edges * sizeof(Edge));

    int idx = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            edges[idx].dist_sq = dist_squared(&points[i], &points[j]);
            edges[idx].i = i;
            edges[idx].j = j;
            idx++;
        }
    }

    qsort(edges, total_edges, sizeof(Edge), compare_edges);
    *edge_count = total_edges;
    return edges;
}

void dfs(int node, int **adj, int *adj_count, int *visited, int *component, int *comp_size)
{
    visited[node] = 1;
    component[(*comp_size)++] = node;

    for (int i = 0; i < adj_count[node]; i++)
    {
        int neighbor = adj[node][i];
        if (!visited[neighbor])
        {
            dfs(neighbor, adj, adj_count, visited, component, comp_size);
        }
    }
}

int *find_component_sizes(int n, Edge *edges, int num_edges, int *num_components)
{
    int **adj = malloc(n * sizeof(int *));
    int *adj_count = calloc(n, sizeof(int));
    int *adj_capacity = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        adj_capacity[i] = 4;
        adj[i] = malloc(adj_capacity[i] * sizeof(int));
    }

    for (int e = 0; e < num_edges; e++)
    {
        int i = edges[e].i;
        int j = edges[e].j;

        if (adj_count[i] >= adj_capacity[i])
        {
            adj_capacity[i] *= 2;
            adj[i] = realloc(adj[i], adj_capacity[i] * sizeof(int));
        }
        adj[i][adj_count[i]++] = j;

        if (adj_count[j] >= adj_capacity[j])
        {
            adj_capacity[j] *= 2;
            adj[j] = realloc(adj[j], adj_capacity[j] * sizeof(int));
        }
        adj[j][adj_count[j]++] = i;
    }

    int *visited = calloc(n, sizeof(int));
    int *component = malloc(n * sizeof(int));
    int *comp_sizes = malloc(n * sizeof(int));
    int comp_idx = 0;

    for (int node = 0; node < n; node++)
    {
        if (!visited[node])
        {
            int comp_size = 0;
            dfs(node, adj, adj_count, visited, component, &comp_size);
            comp_sizes[comp_idx++] = comp_size;
        }
    }

    *num_components = comp_idx;

    for (int i = 0; i < n; i++)
    {
        free(adj[i]);
    }
    free(adj);
    free(adj_count);
    free(adj_capacity);
    free(visited);
    free(component);

    return comp_sizes;
}

UnionFind *create_union_find(int n)
{
    UnionFind *uf = malloc(sizeof(UnionFind));
    uf->parent = malloc(n * sizeof(int));
    uf->size = malloc(n * sizeof(int));
    uf->n = n;
    uf->comp_count = n;

    for (int i = 0; i < n; i++)
    {
        uf->parent[i] = i;
        uf->size[i] = 1;
    }

    return uf;
}

int find(UnionFind *uf, int x)
{
    if (uf->parent[x] != x)
    {
        uf->parent[x] = find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

int unite(UnionFind *uf, int x, int y)
{
    int rx = find(uf, x);
    int ry = find(uf, y);

    if (rx == ry)
        return 0;

    if (uf->size[rx] < uf->size[ry])
    {
        int temp = rx;
        rx = ry;
        ry = temp;
    }

    uf->parent[ry] = rx;
    uf->size[rx] += uf->size[ry];
    uf->comp_count--;
    return 1;
}

void free_union_find(UnionFind *uf)
{
    free(uf->parent);
    free(uf->size);
    free(uf);
}

int compare_desc(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a);
}

long long part1(char **lines, int n, [[maybe_unused]] int m)
{
    Point *points = parse_points(lines, n);

    int edge_count;
    Edge *edges = create_sorted_edges(points, n, &edge_count);

    int num_edges_to_use = 1000;
    if (num_edges_to_use > edge_count)
    {
        num_edges_to_use = edge_count;
    }

    int num_components;
    int *comp_sizes = find_component_sizes(n, edges, num_edges_to_use, &num_components);

    qsort(comp_sizes, num_components, sizeof(int), compare_desc);

    long long result = (long long)comp_sizes[0] * comp_sizes[1] * comp_sizes[2];

    free(points);
    free(edges);
    free(comp_sizes);

    return result;
}

long long part2(char **lines, int n, [[maybe_unused]] int m)
{
    Point *points = parse_points(lines, n);

    int edge_count;
    Edge *edges = create_sorted_edges(points, n, &edge_count);

    UnionFind *uf = create_union_find(n);

    int last_i = -1, last_j = -1;

    for (int e = 0; e < edge_count; e++)
    {
        if (unite(uf, edges[e].i, edges[e].j))
        {
            if (uf->comp_count == 1)
            {
                last_i = edges[e].i;
                last_j = edges[e].j;
                break;
            }
        }
    }

    long long result = (long long)points[last_i].x * points[last_j].x;

    free(points);
    free(edges);
    free_union_find(uf);

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
