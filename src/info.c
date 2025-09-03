#include "info.h"
#include "value.h"

uint64_t hantek_drc_info_voltage_to_millivolts(const hantek_drc_channel_info* channel_info) {
    return hantek_drc_voltage_to_millivolts(channel_info->voltage, channel_info->multiplier);
}

float hantek_drc_info_voltage_to_volts(const hantek_drc_channel_info* channel_info) {
    return hantek_drc_voltage_to_volts(channel_info->voltage, channel_info->multiplier);
}

uint64_t hantek_drc_info_timediv_to_nanos(const hantek_drc_info* info) {
    return hantek_drc_timediv_to_nanos(info->timediv);
}

float hantek_drc_info_timediv_to_seconds(const hantek_drc_info* info) {
    return hantek_drc_timediv_to_seconds(info->timediv);
}

uint64_t hantek_drc_info_milli_sampling_rate(const hantek_drc_info* info) {
    return hantek_drc_generic_milli_sampling_rate(info->caps->x_divisions, info->caps->max_sampling_rate, info->timediv);
}

float hantek_drc_info_sampling_rate(const hantek_drc_info* info) {
    return hantek_drc_generic_sampling_rate(info->caps->x_divisions, info->caps->max_sampling_rate, info->timediv);
}

int64_t hantek_drc_info_max_millivolts(const hantek_drc_channel_info* channel_info) {
    return hantek_drc_max_millivolts(
        channel_info->info->caps->y_divisions,
        channel_info->voltage, 
        channel_info->multiplier);
}

float hantek_drc_info_data_to_volts_multiplier(const hantek_drc_channel_info* channel_info) {
    return hantek_drc_data_to_volts_multiplier(
        channel_info->info->caps->y_divisions,
        channel_info->voltage, 
        channel_info->multiplier);
}

float hantek_drc_info_data_to_volts(const hantek_drc_channel_info* channel_info, int16_t raw_data) {
    return hantek_drc_data_to_volts(
        channel_info->info->caps->y_divisions,
        channel_info->voltage, 
        channel_info->multiplier, 
        raw_data);
}