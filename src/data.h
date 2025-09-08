#pragma once

#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "info.h"

typedef enum hantek_drc_data_type {
    HANTEK_DRC_DATA_TYPE_F32,    
    HANTEK_DRC_DATA_TYPE_F64,
    HANTEK_DRC_DATA_TYPE_U8,
    HANTEK_DRC_DATA_TYPE_U16,
    HANTEK_DRC_DATA_TYPE_U32,
    HANTEK_DRC_DATA_TYPE_U64,
    HANTEK_DRC_DATA_TYPE_I8,
    HANTEK_DRC_DATA_TYPE_I16,
    HANTEK_DRC_DATA_TYPE_I32,
    HANTEK_DRC_DATA_TYPE_I64
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
    hantek_drc_data_value (*map)(hantek_drc_channel* channel, struct hantek_drc_data_fn* fn, int16_t data);
    void* payload;
} hantek_drc_data_fn;

hantek_drc_data_value hantek_drc_data_millivolts(hantek_drc_channel* channel, hantek_drc_data_fn* fn, int16_t data);
void* hantek_drc_data_frame(hantek_drc_channel* channel, hantek_drc_data_fn* fn, const int16_t* frame);
