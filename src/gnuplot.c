#include "gnuplot.h"
#include <string.h>

void hit_enter(void) {
    printf("Hit Enter to continue");
    int ch;
    do {
        ch = getchar();
    } while (ch != '\n' && ch != EOF);
}

bool hantek_drc_gnuplot_prepare(void* params_any, const hantek_drc_info* info) {
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    (void)info;

    if (params->pipe == NULL) {
        params->pipe = popen("gnuplot", "w");
        if (params->pipe == NULL) {
            return false;
        }
        params->should_close = true;
    }

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

bool hantek_drc_gnuplot_finish(void* params_any, const hantek_drc_info* info) {
    (void)info;
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    
    fputs("EOD\nset clip two\nset grid\n", params->pipe);
    fprintf(params->pipe, "set title \"%s (%zu frames, %.2fHz)\"\n", 
        params->drc_path, 
        info->frame_count, 
        hantek_drc_info_sampling_rate_milli(info) / 1000.f
    );

    bool channel_title[HANTEK_DRC_MAX_CHANNELS] = {0};
    for (size_t i = 0; i < params->frame_total; ++i) {
        size_t channel_index = params->frame_channels[i];
        size_t channel_number = info->channel[channel_index].number + 1;
        
        if (i == 0) {
            fprintf(params->pipe, "plot $DRC");
        } else {
            fprintf(params->pipe, ",\"\"");
        }

        fprintf(params->pipe, " index %zu with linespoints", i);
        if (!channel_title[channel_index]) {
            fprintf(params->pipe, " title \"CH%zu\"", channel_number);
            channel_title[channel_index] = true;
        } else {
            fputs(" notitle", params->pipe);
        }
        fprintf(params->pipe, " lt %zu pt 7", channel_number);
    }
    fputc('\n', params->pipe);
    fflush(params->pipe);
    hit_enter();
    return true;
}

void hantek_drc_gnuplot_free(void* params_any, const hantek_drc_info* info) {
    (void)info;
    hantek_drc_gnuplot_params* params = (hantek_drc_gnuplot_params*) params_any;
    if (params->should_close && params->pipe != NULL) {
        pclose(params->pipe);
        params->pipe = NULL;
        params->should_close = false;
    }
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
        .on_finish = &hantek_drc_gnuplot_finish,
        .on_free = &hantek_drc_gnuplot_free
    };
}

HANTEK_DRC_HANDLER_ALLOC_IMPL( gnuplot, frame )
