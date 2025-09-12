#pragma once

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_filter_params {
    hantek_drc_frame_handler filter;
    hantek_drc_frame_handler handler;  
} hantek_drc_filter_params;

hantek_drc_frame_handler hantek_drc_filter_handler(hantek_drc_filter_params* params);
HANTEK_DRC_HANDLER_ALLOC_DEF(filter, frame);
bool hantek_drc_filter_prepare(void* params_any, const hantek_drc_info* info);
bool hantek_drc_filter_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* buffer);
void hantek_drc_filter_free(void* params_any, const hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
