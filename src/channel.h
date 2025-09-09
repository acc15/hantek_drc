#pragma once

#include <stddef.h>
#include <stdint.h>

#include "coupling.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_info hantek_drc_info;

typedef struct hantek_drc_channel {
    size_t index; // zero-based index of enabled channel
    size_t number; // zero-based channel number (0 - 1CH, 1 - 2CH, etc)
    uint16_t voltage; // voltage index
    uint16_t multiplier; // multiplier index (most cases - 10^multiplier)
    hantek_drc_coupling coupling;
    uint16_t lever; // signal y offset (lever) - values [0..255]
    hantek_drc_info* info;
} hantek_drc_channel;

int64_t hantek_drc_channel_volts_milli(const hantek_drc_channel* channel);
int64_t hantek_drc_channel_max_volts_milli(const hantek_drc_channel* channel);

#ifdef __cplusplus
}
#endif
