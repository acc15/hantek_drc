#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include <getopt.h>

#include <hantek_drc/range.h>
#include <hantek_drc/format.h>
#include <hantek_drc/channel.h>
#include <hantek_drc/read.h>
#include <hantek_drc/filter.h>
#include <hantek_drc/gnuplot.h>

#ifndef PROGRAM_NAME
#   define PROGRAM_NAME "hantek_drc_gnuplot"
#endif

int usage(void) {
    printf("Usage:\n\n" PROGRAM_NAME " <drc-path> [-c <channel-range>] [-f <frame-range>]\n");
    return -1;
}

int main(int argc, char *argv[]) {
    hantek_drc_range_filter_params range = {0};

    int opt;
    while ((opt = getopt(argc, argv, "c:f:")) != -1) {
        switch (opt) {
        case 'c':
            range.channel = hantek_drc_parse_range(optarg);
            break;
        case 'f':
            range.frame = hantek_drc_parse_range(optarg);
            break;
        default:
            return usage();
        }
    }
    if (optind >= argc) {
        return usage();
    }

    hantek_drc_data_format_params format = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_gnuplot_params gnuplot = {
        .drc_path = argv[optind],
        .format = hantek_drc_data_format(&format)
    };
    hantek_drc_filter_params filter = {
        .filter = hantek_drc_range_filter(&range),
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
