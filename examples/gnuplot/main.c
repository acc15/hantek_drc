#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../src/range.h"
#include "../../src/format.h"
#include "../../src/channel.h"
#include "../../src/read.h"
#include "../../src/filter.h"
#include "../../src/gnuplot.h"

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

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc < 2) {
        printf("Usage:\n\n" PROGRAM_NAME " <drc-path> [channel-range] [frame-range]\n");
        return -1;
    }

    hantek_drc_range_filter_params range_filter = {
        .channel = parse_range(argc > 2 ? argv[2] : NULL),
        .frame = parse_range(argc > 3 ? argv[3] : NULL)
    };
    hantek_drc_data_format_params format = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_gnuplot_params gnuplot = {
        .drc_path = argv[1],
        .format = hantek_drc_data_format(&format)
    };
    hantek_drc_filter_params filter = {
        .filter = hantek_drc_range_filter(&range_filter),
        .handler = hantek_drc_gnuplot(&gnuplot)
    };
    hantek_drc_info info = {
        .caps = hantek_drc_6254bd(),
        .handler = hantek_drc_filter(&filter)
    };
    if (!hantek_drc_read_file(gnuplot.drc_path, &info)) {
        fprintf(stderr, "Can't read DRC file \"%s\"\n", gnuplot.drc_path);
        return -1;
    }
    if (!hantek_drc_info_finish_free(&info)) {
        fprintf(stderr, "Can't finish gnuplot, file \"%s\"", gnuplot.drc_path);
        return -1;
    }
    return 0;
}
