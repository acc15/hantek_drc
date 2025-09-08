#include "info.h"
#include "value.h"
#include <memory.h>
#include <stdlib.h>

struct hantek_drc_info hantek_drc_init_6254bd(void) {
    return (struct hantek_drc_info) {
        .max_channels = 4,
        .max_sampling_rate = 1000000000000ULL,
        .x_div = 10,
        .y_div = 8
    };
}

void hantek_drc_free(struct hantek_drc_info* info) {
    if (info->frame_handler.on_free != NULL) {
        info->frame_handler.on_free(info);
    }
}

uint64_t hantek_drc_channel_volts_milli(const hantek_drc_channel* channel) {
    return hantek_drc_voltage_volts_milli(channel->voltage, channel->multiplier);
}

uint64_t hantek_drc_channel_max_volts_milli(const hantek_drc_channel* channel) {
    return hantek_drc_max_volts_milli(
        channel->info->y_div,
        channel->voltage, 
        channel->multiplier);
}

uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info) {
    return hantek_drc_timediv_nanos(info->timediv);
}

uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info) {
    return hantek_drc_sampling_rate_milli(info->x_div, info->max_sampling_rate, 
        hantek_drc_timediv_nanos(info->timediv));
}
