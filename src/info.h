#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "channel.h"
#include "data.h"
#include "handler.h"
#include "caps.h"

#ifndef HANTEK_DRC_MAX_CHANNELS
#   define HANTEK_DRC_MAX_CHANNELS 8
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_frame_handler {
    struct hantek_drc_handler;
    bool(*on_frame)(hantek_drc_channel* channel, const int16_t* buffer);
    bool(*on_prepare)(hantek_drc_info* info);
} hantek_drc_frame_handler;

typedef struct hantek_drc_info {
    hantek_drc_caps caps;
    hantek_drc_frame_handler frame_handler;
    hantek_drc_data_handler data_handler;
    
    // parsed data
    hantek_drc_channel channel[HANTEK_DRC_MAX_CHANNELS];
    size_t channel_count; // count of enabled channels in file
    uint16_t timediv; // timediv index
    size_t buffer_length; // buffer length (count of data points in single frame)
    size_t frame_count; // count of frames that has been read from files

} hantek_drc_info;

void hantek_drc_free(struct hantek_drc_info* info);

uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info);
uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
