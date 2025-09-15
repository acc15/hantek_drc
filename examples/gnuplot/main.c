#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    size_t* frame_channels; // indexes of channels for each written frame
    size_t frame_total; // count of recorded frames
    size_t frame_allocated; // count of frames allocated
} hantek_drc_gnuplot_params;

bool hantek_drc_gnuplot_prepare(void* params_any, const hantek_drc_info* info) {
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    (void)info;

    return fputs("$DRC << EOD\n", params->pipe) != EOF;
}

bool hantek_drc_gnuplot_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* frame) {
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    
    if (params->frame_allocated <= params->frame_total) {
        size_t frame_allocated = params->frame_allocated < 16 ? 16 : params->frame_allocated * 2;
        size_t* frame_channels = (size_t*) realloc(params->frame_channels, frame_allocated * sizeof(size_t));
        if (frame_channels == NULL) {
            return false;
        }
        memset(frame_channels + params->frame_allocated, 0, (frame_allocated - params->frame_allocated)*sizeof(size_t));
        params->frame_channels = frame_channels;
        params->frame_allocated = frame_allocated;
    }

    size_t x_offset = channel->info->frame_count * channel->info->buffer_length;
    for (size_t i = 0; i < channel->info->buffer_length; ++i) {
        if (
            fprintf(params->pipe, "%zu ", x_offset + i) <= 0 || 
            hantek_drc_format_print(params->pipe, &params->format, channel, frame[i]) <= 0 || 
            fputc('\n', params->pipe) <= 0
        ) {
            return false;
        }
    }

    if (fputs("\n\n", params->pipe) == EOF) {
        return false;
    }

    params->frame_channels[params->frame_total] = channel->index;
    ++params->frame_total;
    return true;
}

void hantek_drc_gnuplot_free(void* params_any, const hantek_drc_info* info) {
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    (void)info;
    
    fputs("EOD\n", params->pipe);

    bool channel_title[HANTEK_DRC_MAX_CHANNELS] = {0};
    for (size_t i = 0; i < params->frame_total; ++i) {
        size_t channel_index = params->frame_channels[i];
        size_t channel_number = info->channel[channel_index].number + 1;
        
        if (i == 0) {
            fprintf(params->pipe, "plot $DRC");
        } else {
            fprintf(params->pipe, ",\"\"");
        }

        fprintf(params->pipe, " index %zu with lines", i);
        if (!channel_title[channel_index]) {
            fprintf(params->pipe, " title \"CH%zu\"", channel_number);
            channel_title[channel_index] = true;
        } else {
            fputs(" notitle", params->pipe);
        }
        fprintf(params->pipe, " linetype %zu", channel_number);
    }
    fputc('\n', params->pipe);
    fflush(params->pipe);
    hit_enter();

    if (params->frame_channels != NULL) {
        free(params->frame_channels);
        params->frame_channels = NULL;
    }
}


hantek_drc_frame_handler hantek_drc_gnuplot(hantek_drc_gnuplot_params* params) {
    return (hantek_drc_frame_handler) {
        .params = params,
        .on_prepare = &hantek_drc_gnuplot_prepare,
        .on_frame = &hantek_drc_gnuplot_frame,
        .on_free = &hantek_drc_gnuplot_free
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
    fputs("set grid\n", gnuplot);
    fprintf(gnuplot, "set title \"%s\"\n", input_path);

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
