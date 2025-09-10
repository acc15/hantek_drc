#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#include "handler.h"

#ifdef __cplusplus
extern "C" {
#endif

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

size_t hantek_drc_data_type_size(hantek_drc_data_type type);

typedef union hantek_drc_data_value {
    uint8_t     u8;
    uint16_t    u16;
    uint32_t    u32;
    uint64_t    u64;
    int8_t      i8;
    int16_t     i16;
    int32_t     i32;
    int64_t     i64;
    float_t     f32;
    double_t    f64;
} hantek_drc_data_value;

typedef struct hantek_drc_format_handler {
    struct hantek_drc_handler;
    hantek_drc_data_type type;
    hantek_drc_data_value (*on_data)(
        const struct hantek_drc_format_handler* format, 
        const hantek_drc_channel* channel, 
        int16_t data
    );
} hantek_drc_format_handler;

hantek_drc_data_value hantek_drc_format_data(
    const hantek_drc_format_handler* format, 
    const hantek_drc_channel* channel, 
    int16_t data
);

void* hantek_drc_format_frame(
    const hantek_drc_format_handler* format, 
    const hantek_drc_channel* channel, 
    const int16_t* frame
);

typedef struct hantek_drc_data_format_params {
    hantek_drc_data_type type;
    int64_t (*multiplier_fn)(const hantek_drc_channel*);
    int64_t (*divider_fn)(const hantek_drc_channel*);
    int64_t multiplier;
    int64_t divider;
    bool positive;
} hantek_drc_data_format_params;

hantek_drc_format_handler hantek_drc_data_format_handler(hantek_drc_data_format_params* params);
HANTEK_DRC_HANDLER_ALLOC_DEF( data_format, format );

void hantek_drc_data_format_scale_free(hantek_drc_info* info);

hantek_drc_data_format_params hantek_drc_data_format_raw(hantek_drc_data_type type);
hantek_drc_data_format_params hantek_drc_data_format_volts_milli(hantek_drc_data_type type);
hantek_drc_data_format_params hantek_drc_data_format_volts(hantek_drc_data_type type);
hantek_drc_data_format_params hantek_drc_data_format_muldiv(
    hantek_drc_data_type type, 
    bool positive,
    int64_t multiplier,
    int64_t divider
);
hantek_drc_data_format_params hantek_drc_data_format_mul(
    hantek_drc_data_type type, 
    bool positive,
    int64_t multiplier
);

hantek_drc_data_value hantek_drc_data_format(
    const hantek_drc_format_handler* format, 
    const hantek_drc_channel* channel, 
    int16_t data
);

#ifdef __cplusplus
}
#endif
