#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef PROGRAM_NAME
#   define PROGRAM_NAME "hantek_drc_gnuplot"
#endif

typedef struct range_t {
    bool valid;
    unsigned int min;
    unsigned int max;
} range_t;

range_t parse_range(const char* str) {
    range_t result = {.valid = false};
    if (str == NULL) {
        return result;
    }
    
    const char* start_ptr = str;
    char* end_ptr;

    result.min = result.max = strtol(start_ptr, &end_ptr, 10);
    if (end_ptr == start_ptr) {
        return result;
    }

    result.valid = true;
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

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc < 2) {
        printf("Usage:\n\n" PROGRAM_NAME " <drc-path> [channel-range] [frame-range]\n");
        return -1;
    }

    const char* input_path = argv[1];
    range_t channel_range = parse_range(argc > 2 ? argv[2] : NULL);
    range_t frame_range = parse_range(argc > 3 ? argv[3] : NULL);

    printf("input path is %s, channel (valid=%d,min=%d,max=%d), frame (valid=%d,min=%d,max=%d)", 
        input_path,
        channel_range.valid, channel_range.min, channel_range.max,
        frame_range.valid, frame_range.min, frame_range.max
    );
    
    return 0;
}
