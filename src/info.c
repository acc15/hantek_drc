#include "info.h"
#include "value.h"
#include <memory.h>

void hantek_drc_init(struct hantek_drc_info* info) {
    memset(info, 0, sizeof(hantek_drc_info));
}

void hantek_drc_init_6254bd(struct hantek_drc_info* info) {
    hantek_drc_init(info);
    info->max_channels = 4;
    info->max_sampling_rate = 1000000000000ULL;
    info->x_div = 10;
    info->y_div = 8;
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

float hantek_drc_channel_data_volts_multiplier(const hantek_drc_channel* channel) {
    return hantek_drc_data_volts_multiplier(
        channel->info->y_div,
        channel->voltage, 
        channel->multiplier);
}

float hantek_drc_channel_data_volts(const hantek_drc_channel* channel, int16_t data) {
    return hantek_drc_data_volts(
        channel->info->y_div,
        channel->voltage, 
        channel->multiplier, 
        data);
}

float hantek_drc_channel_data_normalize(const hantek_drc_channel* channel, int16_t data) {
    return (float) data / INT16_MAX;
}

hantek_drc_data_fn hantek_drc_data_fn_or_default(hantek_drc_data_fn data_fn) {
    return data_fn != NULL ? data_fn : hantek_drc_channel_data_volts;
}
