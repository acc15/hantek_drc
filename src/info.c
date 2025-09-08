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
    if (info->on_free != NULL) {
        info->on_free(info);
    }
}

uint64_t hantek_drc_channel_millivolts(const hantek_drc_channel* channel) {
    return hantek_drc_voltage_millivolts(channel->voltage, channel->multiplier);
}

float hantek_drc_channel_volts(const hantek_drc_channel* channel) {
    return hantek_drc_voltage_volts(channel->voltage, channel->multiplier);
}

uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info) {
    return hantek_drc_timediv_nanos(info->timediv);
}

float hantek_drc_info_timediv_seconds(const hantek_drc_info* info) {
    return hantek_drc_timediv_seconds(info->timediv);
}

uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info) {
    return hantek_drc_sampling_rate_milli(info->x_div, info->max_sampling_rate, 
        hantek_drc_timediv_nanos(info->timediv));
}

float hantek_drc_info_sampling_rate(const hantek_drc_info* info) {
    return hantek_drc_sampling_rate(info->x_div, info->max_sampling_rate, hantek_drc_timediv_nanos(info->timediv));
}

int64_t hantek_drc_channel_max_millivolts(const hantek_drc_channel* channel) {
    return hantek_drc_max_millivolts(
        channel->info->y_div,
        channel->voltage, 
        channel->multiplier);
}
