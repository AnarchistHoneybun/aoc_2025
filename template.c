#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aoc_utils.h"

void part1(char **lines, int n, int m) {
    int result = 0;
    
    // Access input: lines[i] contains line i (zero-indexed)
    // Example usage:
    //   char *first_line = lines[0];
    //   for (int i = 0; i < n; i++) { ... }
    //
    // Parse numbers from a line:
    //   char *s = lines[i];
    //   long long num = parse_number(&s);  // advances s past the number
    
    for (int i = 0; i < n; i++) {
        char *line = lines[i];
        
        // TODO: Implement part 1 solution
    }
    
    printf("Part 1: %d\n", result);
}

void part2(char **lines, int n, int m) {
    int result = 0;
    
    for (int i = 0; i < n; i++) {
        char *line = lines[i];
        
        // TODO: Implement part 2 solution
    }
    
    printf("Part 2: %d\n", result);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    // Read entire input file into memory
    // n = number of lines, m = length of longest line
    int n, m;
    char **lines = read_input(argv[1], &n, &m);
    if (lines == NULL) {
        return 1;
    }
    
    part1(lines, n, m);
    part2(lines, n, m);
    
    free_input(lines);
    
    return 0;
}
