#pragma once

#include <stdio.h>

#include "info.h"
#include "frame_handler.h"
#include "format.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_gnuplot_params {
    const char* drc_path;
    FILE* pipe;
    bool should_close;
    hantek_drc_format_handler format;
    size_t* frame_channels; // indexes of channels for each written frame
    size_t frame_total; // count of recorded frames
    size_t frame_allocated; // count of frames allocated
} hantek_drc_gnuplot_params;

bool hantek_drc_gnuplot_prepare(void* params_any, const hantek_drc_info* info);
bool hantek_drc_gnuplot_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* frame);
bool hantek_drc_gnuplot_finish(void* params_any, const hantek_drc_info* info);
void hantek_drc_gnuplot_free(void* params_any, const hantek_drc_info* info);
hantek_drc_frame_handler hantek_drc_gnuplot(hantek_drc_gnuplot_params* params);
HANTEK_DRC_HANDLER_ALLOC_DEF( gnuplot, frame );

#ifdef __cplusplus
}
#endif
