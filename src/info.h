#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef HANTEK_DRC_MAX_CHANNELS
#   define HANTEK_DRC_MAX_CHANNELS 8
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hantek_drc_coupling {
    HANTEK_DRC_COUPLING_DC,
    HANTEK_DRC_COUPLING_AC,
    HANTEK_DRC_COUPLING_GND
} hantek_drc_coupling;

struct hantek_drc_info;

typedef struct hantek_drc_channel {
    size_t index; // zero-based index of enabled channel
    size_t number; // zero-based channel number (0 - 1CH, 1 - 2CH, etc)
    uint16_t voltage; // voltage index
    uint16_t multiplier; // multiplier index (most cases - 10^multiplier)
    hantek_drc_coupling coupling;
    uint16_t lever; // signal y offset (lever) - values [0..255]
    struct hantek_drc_info* info;
} hantek_drc_channel;

typedef struct hantek_drc_info {
    
    size_t max_channels;
    uint64_t max_sampling_rate;
    size_t x_div;
    size_t y_div;

    hantek_drc_channel channel[HANTEK_DRC_MAX_CHANNELS];

    // callbacks
    bool(*on_frame)(struct hantek_drc_channel* channel, const int16_t* buffer);
    bool(*on_prepare)(struct hantek_drc_info* info);
    void(*on_free)(struct hantek_drc_info* info);
    void* payload; // additional custom info for callbacks
    
    // parsed data
    uint16_t timediv; // timediv index
    size_t buffer_length; // buffer length (count of data points in single frame)
    size_t channel_count; // count of enabled channels in file
    size_t frame_count; // count of frames that has been read from files

} hantek_drc_info;

void hantek_drc_init(struct hantek_drc_info* info);
void hantek_drc_init_6254bd(struct hantek_drc_info* info);
void hantek_drc_free(struct hantek_drc_info* info);

uint64_t hantek_drc_channel_millivolts(const hantek_drc_channel* channel);
float hantek_drc_channel_volts(const hantek_drc_channel* channel);
uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info);
float hantek_drc_info_timediv_seconds(const hantek_drc_info* info);
uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info);
float hantek_drc_info_sampling_rate(const hantek_drc_info* info);
int64_t hantek_drc_channel_max_millivolts(const hantek_drc_channel* channel);
float hantek_drc_channel_data_volts_multiplier(const hantek_drc_channel* channel);
float hantek_drc_channel_data_volts(const hantek_drc_channel* channel, int16_t data);
float hantek_drc_channel_data_normalize(const hantek_drc_channel* channel, int16_t data);

// converts single point of data
typedef float (*hantek_drc_data_fn)(const hantek_drc_channel* channel, int16_t data);
hantek_drc_data_fn hantek_drc_data_fn_or_default(hantek_drc_data_fn data_fn);

// converts whole frame to arbitrary data type points
typedef void* (*hantek_drc_frame_fn)(const hantek_drc_channel* channel, const int16_t* data);
void* hantek_drc_channel_frame_float_volts(const hantek_drc_channel* channel, const int16_t* data);
void* hantek_drc_channel_frame_uint16_10bit(const hantek_drc_channel* channel, const int16_t* data);

hantek_drc_frame_fn hantek_drc_frame_fn_or_default(hantek_drc_frame_fn fn);

#ifdef __cplusplus
}
#endif
