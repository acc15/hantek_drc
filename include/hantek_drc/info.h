#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "channel.h"
#include "frame_handler.h"
#include "caps.h"

#ifndef HANTEK_DRC_MAX_CHANNELS
#   define HANTEK_DRC_MAX_CHANNELS 8
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_info {
    hantek_drc_caps caps;
    hantek_drc_frame_handler handler;
    
    // parsed data
    hantek_drc_channel channel[HANTEK_DRC_MAX_CHANNELS];
    size_t channel_count; // count of enabled channels in file
    uint16_t timediv; // timediv index
    size_t buffer_length; // buffer length (count of data points in single frame)
    size_t frame_count; // total count of frames that has been read from files

} hantek_drc_info;

uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info);
uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info);

bool hantek_drc_info_finish(struct hantek_drc_info* info);
void hantek_drc_info_free(struct hantek_drc_info* info);
bool hantek_drc_info_finish_free(struct hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
