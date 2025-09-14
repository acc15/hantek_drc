#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../../src/range.h"
#include "../../src/format.h"
#include "../../src/channel.h"
#include "../../src/read.h"

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

typedef struct hantek_drc_gnuplot_params {
    FILE* pipe;
    hantek_drc_format_handler format;
} hantek_drc_gnuplot_params;

bool hantek_drc_gnuplot_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* frame) {
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    for (size_t i = 0; i < channel->info->buffer_length; ++i) {
        fprintf(params->pipe, "%zu,", channel->number);
        hantek_drc_data_print(params->pipe, 
            hantek_drc_format_type(&params->format, channel), 
            hantek_drc_format_data(&params->format, channel, frame[i])
        );
        fputc('\n', params->pipe);
    }
    fputs("e\n", params->pipe);
    return true;
}

bool hantek_drc_gnuplot_frame_prepare(void* params_any, const hantek_drc_info* info) {
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    fprintf(params->pipe, "set title \"Frame %zu\"\n", info->frame_count);
    for (size_t i = 0; i < info->channel_count; ++i) {
        if (i == 0) {
            fprintf(params->pipe, "plot \"-\"");
        } else {
            fprintf(params->pipe, ",\"\"");
        }
        fprintf(params->pipe, " with lines title \"CH%zu\"", (i + 1));
    }
    fputc('\n', params->pipe);
    return true;
}

bool hantek_drc_gnuplot_frame_finish(void* params_any, const hantek_drc_info* info) {
    (void)info;
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    fflush(params->pipe);
    hit_enter();
    return true;
}

hantek_drc_frame_handler hantek_drc_gnuplot(hantek_drc_gnuplot_params* params) {
    return (hantek_drc_frame_handler) {
        .params = &params,
        .on_frame = &hantek_drc_gnuplot_frame,
        .on_frame_prepare = &hantek_drc_gnuplot_frame_prepare,
        .on_frame_finish = &hantek_drc_gnuplot_frame_finish,
    };
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

    hantek_drc_data_format_params format = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_gnuplot_params params = {
        .pipe = gnuplot,
        .format = hantek_drc_data_format(&format)
    };
    hantek_drc_info info = {
        .caps = hantek_drc_6254bd(),
        .handler = hantek_drc_gnuplot(&params)
    };
    if (!hantek_drc_read_file(input_path, &info)) {
        fprintf(stderr, "Can't read DRC file \"%s\"\n", input_path);
        return -1;
    }
    hantek_drc_info_free(&info);

    pclose(gnuplot);
    return 0;
}
