#pragma once

#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "info.h"

typedef enum hantek_drc_data_type {
    HANTEK_DRC_DATA_TYPE_U8,
    HANTEK_DRC_DATA_TYPE_U16,
    HANTEK_DRC_DATA_TYPE_U32,
    HANTEK_DRC_DATA_TYPE_U64,
    HANTEK_DRC_DATA_TYPE_I8,
    HANTEK_DRC_DATA_TYPE_I16,
    HANTEK_DRC_DATA_TYPE_I32,
    HANTEK_DRC_DATA_TYPE_I64,
    HANTEK_DRC_DATA_TYPE_F32,    
    HANTEK_DRC_DATA_TYPE_F64,
} hantek_drc_data_type;

size_t hantek_drc_data_type_size(enum hantek_drc_data_type type);

typedef union hantek_drc_data_value {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float_t f32;
    double_t f64;
} hantek_drc_data_value;

typedef struct hantek_drc_data_fn {
    hantek_drc_data_type type;
    hantek_drc_data_value (*map)(struct hantek_drc_channel* channel, struct hantek_drc_data_fn* fn, int16_t data);
    void* payload;
} hantek_drc_data_fn;

hantek_drc_data_value hantek_drc_data_millivolts(struct hantek_drc_channel* channel, struct hantek_drc_data_fn* fn, int16_t data) {
    int64_t mv = data * hantek_drc_channel_max_millivolts(channel);
    if (fn->type == HANTEK_DRC_DATA_TYPE_F32) {
        return (hantek_drc_data_value) {.f32 = (float_t) mv / INT16_MAX};
    }
    if (fn->type == HANTEK_DRC_DATA_TYPE_F32) {
        return (hantek_drc_data_value) {.f64 = (double_t) mv / INT16_MAX};
    }

    mv /= INT16_MAX;
    switch (fn->type) {
    case HANTEK_DRC_DATA_TYPE_U8:  return (hantek_drc_data_value) {.u8 = (uint8_t) mv};
    case HANTEK_DRC_DATA_TYPE_U16: return (hantek_drc_data_value) {.u16 = (uint16_t) mv};
    case HANTEK_DRC_DATA_TYPE_U32: return (hantek_drc_data_value) {.u32 = (uint32_t) mv};
    case HANTEK_DRC_DATA_TYPE_U64: return (hantek_drc_data_value) {.u64 = (uint64_t) mv};
    case HANTEK_DRC_DATA_TYPE_I8:  return (hantek_drc_data_value) {.i8 = (int8_t) mv};
    case HANTEK_DRC_DATA_TYPE_I16: return (hantek_drc_data_value) {.i16 = (int16_t) mv};
    case HANTEK_DRC_DATA_TYPE_I32: return (hantek_drc_data_value) {.i32 = (int32_t) mv};
    default: return (hantek_drc_data_value) {.i64 = mv};
    }
}

void* hantek_drc_data_frame(struct hantek_drc_channel* channel, struct hantek_drc_data_fn* fn, const int16_t* frame);
