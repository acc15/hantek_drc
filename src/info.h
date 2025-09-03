#pragma once

#include "caps.h"
typedef enum hantek_drc_coupling {
    HANTEK_DRC_COUPLING_DC,
    HANTEK_DRC_COUPLING_AC,
    HANTEK_DRC_COUPLING_GND
} hantek_drc_coupling;

struct hantek_drc_info;

typedef struct hantek_drc_channel_info {
    size_t index; // zero-based index of enabled channel
    size_t number; // zero-based channel number (0 - 1CH, 1 - 2CH, etc)
    uint16_t voltage; // voltage index
    uint16_t multiplier; // multiplier index (most cases - 10^multiplier)
    hantek_drc_coupling coupling;
    uint16_t lever; // signal y offset (lever) - values [0..255]
    struct hantek_drc_info* info;
} hantek_drc_channel_info;

typedef struct hantek_drc_info {
    
    hantek_drc_device_capabilities* caps;
    hantek_drc_channel_info* channel_info; // array with size = caps->max_channels
    
    void(*on_frame)(
        struct hantek_drc_channel_info* channel_info, 
        const int16_t* buffer, 
        size_t frame_index
    );
    void(*on_before_data)(struct hantek_drc_info* info);
    void(*on_free)(struct hantek_drc_info* info);
    void* payload; // additional custom info for callbacks
    
    uint16_t timediv; // timediv index
    size_t buffer_length; // buffer length (count of data points in single frame)
    size_t channel_count; // count of enabled channels in file
    size_t frame_count; // count of frames per single channel (total frame count = frame_count * channel_count)

} hantek_drc_info;

// void hantek_drc_init_6254bd(struct hantek_drc_info* info);
// void hantek_drc_free(struct hantek_drc_info* info);

uint64_t hantek_drc_info_voltage_to_millivolts(const hantek_drc_channel_info* channel_info);
float hantek_drc_info_voltage_to_volts(const hantek_drc_channel_info* channel_info);
uint64_t hantek_drc_info_timediv_to_nanos(const hantek_drc_info* info);
float hantek_drc_info_timediv_to_seconds(const hantek_drc_info* info);
uint64_t hantek_drc_info_milli_sampling_rate(const hantek_drc_info* info);
float hantek_drc_info_sampling_rate(const hantek_drc_info* info);
int64_t hantek_drc_info_max_millivolts(const hantek_drc_channel_info* channel_info);
float hantek_drc_info_data_to_volts_multiplier(const hantek_drc_channel_info* channel_info);
float hantek_drc_info_data_to_volts(const hantek_drc_channel_info* channel_info, int16_t raw_data);