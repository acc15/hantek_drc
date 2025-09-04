#pragma once

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_mem_payload {
    hantek_drc_frame_fn frame_fn;
    void*** data;
    size_t frames_allocated;
} hantek_drc_mem_payload;

bool hantek_drc_mem_prepare(hantek_drc_info* info);
bool hantek_drc_mem_frame(hantek_drc_channel* channel, const int16_t* buffer);
bool hantek_drc_mem_init(hantek_drc_info* info, hantek_drc_frame_fn frame_fn);
void hantek_drc_mem_free(hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
