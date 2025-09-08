#include "info.h"
#include "value.h"

uint64_t hantek_drc_channel_volts_milli(const hantek_drc_channel* channel) {
    return hantek_drc_voltage_volts_milli(channel->voltage, channel->multiplier);
}

uint64_t hantek_drc_channel_max_volts_milli(const hantek_drc_channel* channel) {
    return hantek_drc_max_volts_milli(
        channel->info->y_div,
        channel->voltage, 
        channel->multiplier);
}
