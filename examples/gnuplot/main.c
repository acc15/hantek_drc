#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../../src/range.h"

#ifndef PROGRAM_NAME
#   define PROGRAM_NAME "hantek_drc_gnuplot"
#endif

hantek_drc_range parse_range(const char* str) {
    hantek_drc_range result = {.enabled = false};
    if (str == NULL) {
        return result;
    }
    
    const char* start_ptr = str;
    char* end_ptr;

    result.min = result.max = strtol(start_ptr, &end_ptr, 10);
    if (end_ptr == start_ptr) {
        return result;
    }

    result.enabled = true;
    if (*end_ptr == 0) {
        return result;
    }

    if (*end_ptr == '-') {
        start_ptr = end_ptr + 1;
    }
    result.max = strtol(start_ptr, &end_ptr, 10);
    if (end_ptr == start_ptr) {
        result.max = result.min;
    }
    return result;
}

void print_range(const hantek_drc_range* range) {
    printf("enabled=%d,min=%zu,max=%zu", range->enabled, range->min, range->max);
}

void hit_enter(void) {
    printf("Hit Enter to continue");
    int ch;
    do {
        ch = getchar();
    } while (ch != '\n' && ch != EOF);
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc < 2) {
        printf("Usage:\n\n" PROGRAM_NAME " <drc-path> [channel-range] [frame-range]\n");
        return -1;
    }

    const char* input_path = argv[1];
    hantek_drc_range channel_range = parse_range(argc > 2 ? argv[2] : NULL);
    hantek_drc_range frame_range = parse_range(argc > 3 ? argv[3] : NULL);
    printf("input path is %s, channel (", input_path);
    print_range(&channel_range);
    printf("), frame (");
    print_range(&frame_range);
    printf(")\n");

    FILE* gnuplot = popen("gnuplot", "w");
    if (gnuplot == NULL) {
        return -1;
    }
    fprintf(gnuplot, "set grid\n");

    fprintf(gnuplot, "plot sin(x)\n");
    fflush(gnuplot);
    hit_enter();

    fprintf(gnuplot, "plot cos(x)\n");
    fflush(gnuplot);
    hit_enter();

    pclose(gnuplot);
    return 0;
}
