#pragma once

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_mem_channel {
    void** frames;
    size_t recorded;
    size_t allocated;
} hantek_drc_mem_channel;

typedef struct hantek_drc_mem_params {
    hantek_drc_mem_channel channels[HANTEK_DRC_MAX_CHANNELS];
} hantek_drc_mem_params;

bool hantek_drc_mem_prepare(hantek_drc_info* info);
bool hantek_drc_mem_frame(hantek_drc_channel* channel, const int16_t* buffer);
bool hantek_drc_mem_alloc(hantek_drc_info* info);
bool hantek_drc_mem_ext(hantek_drc_info* info, hantek_drc_mem_params* params);
void hantek_drc_mem_free(hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
