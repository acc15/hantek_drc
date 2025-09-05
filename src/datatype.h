#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hantek_drc_data_type {
    HANTEK_DRC_U8,
    HANTEK_DRC_U16,
    HANTEK_DRC_U32,
    HANTEK_DRC_U64,
    HANTEK_DRC_I8,
    HANTEK_DRC_I16,
    HANTEK_DRC_I32,
    HANTEK_DRC_I64,
    HANTEK_DRC_F32,
    HANTEK_DRC_F64,
} hantek_drc_data_type;

size_t hantek_drc_data_type_size(hantek_drc_data_type type) {
    switch (type)
    {
    case HANTEK_DRC_U8: return sizeof(uint8_t);
    case HANTEK_DRC_U16: return sizeof(uint16_t);
    case HANTEK_DRC_U32: return sizeof(uint32_t);
    case HANTEK_DRC_U64: return sizeof(uint64_t);
    case HANTEK_DRC_I8: return sizeof(int8_t);
    case HANTEK_DRC_I16: return sizeof(int16_t);
    case HANTEK_DRC_I32: return sizeof(int32_t);
    case HANTEK_DRC_I64: return sizeof(int64_t);
    case HANTEK_DRC_F32: return sizeof(float);
    case HANTEK_DRC_F64: return sizeof(double);
    default: return 0;
    }
}

typedef union hantek_drc_value_type {
    uint8_t     u8;
    uint16_t    u16;
    uint32_t    u32;
    uint64_t    u64;
    int8_t      i8;
    int16_t     i16;
    int32_t     i32;
    int64_t     i64;
    float       f32;
    double      f64;
} hantek_drc_value_type;

typedef struct hantek_drc_data_value {
    hantek_drc_data_type type;
    hantek_drc_value_type value;
};

typedef struct hantek_drc_data_fn {
    hantek_drc_data_type type;
    void (*on_value)(const hantek_drc_channel* channel, int16_t value, hantek_drc_data_type type, hantek_drc_value_type* out);
} hantek_drc_data_fn;

hantek_drc_data_fn hantek_drc_data_volts(hantek_drc_data_value type_scale) {
    return hantek_drc_data_fn {
        .type = type_scale.type,
        .on_value = 
    }
}

void* hantek_drc_data_frame(const hantek_drc_channel* channel, int16_t* buffer, const hantek_drc_data_fn* fn) {
    size_t data_type_size = hantek_drc_data_type_size(fn->type);
    void* buf = malloc(data_type_size*channel->info->buffer_length);
    if (buf == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < channel->info->buffer_length; ++i) {
        hantek_drc_value_type* out = ((uint8_t*)buf + (data_type_size * i));
        fn->on_value(channel, buffer[i], fn->type, out);
    }
    return buf;
}

#ifdef __cplusplus
}
#endif