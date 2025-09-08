#include "data.h"

#include <stdlib.h>
#include <string.h>

size_t hantek_drc_data_type_size(enum hantek_drc_data_type type) {
    switch (type) {
        case HANTEK_DRC_DATA_TYPE_U8: return sizeof(uint8_t);
        case HANTEK_DRC_DATA_TYPE_U16: return sizeof(uint16_t);
        case HANTEK_DRC_DATA_TYPE_U32: return sizeof(uint32_t);
        case HANTEK_DRC_DATA_TYPE_U64: return sizeof(uint64_t);
        case HANTEK_DRC_DATA_TYPE_I8: return sizeof(int8_t);
        case HANTEK_DRC_DATA_TYPE_I16: return sizeof(int16_t);
        case HANTEK_DRC_DATA_TYPE_I32: return sizeof(int32_t);
        case HANTEK_DRC_DATA_TYPE_I64: return sizeof(int64_t);
        case HANTEK_DRC_DATA_TYPE_F32: return sizeof(float_t);
        case HANTEK_DRC_DATA_TYPE_F64: return sizeof(double_t);
        default: return 0;
    }
}
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

void* hantek_drc_data_frame(struct hantek_drc_channel* channel, struct hantek_drc_data_fn* fn, const int16_t* frame) {
    size_t sz = hantek_drc_data_type_size(fn->type);
    void* data = malloc(sz * channel->info->buffer_length);
    if (data == NULL) {
        return NULL;
    }
    for (size_t i=0; i < channel->info->buffer_length; ++i) {
        hantek_drc_data_value v = fn->map(channel, fn, frame[i]);
        memcpy((unsigned char*)data + (sz * i), &v, sz);
    }
    return data;
}
