#include "data.h"

#include <stdlib.h>
#include <string.h>

#include "info.h"

size_t hantek_drc_data_type_size(hantek_drc_data_type type) {
    switch (type) {
        case HANTEK_DRC_DATA_TYPE_I64: return sizeof(int64_t);
        case HANTEK_DRC_DATA_TYPE_I32: return sizeof(int32_t);
        case HANTEK_DRC_DATA_TYPE_I16: return sizeof(int16_t);
        case HANTEK_DRC_DATA_TYPE_I8:  return sizeof(int8_t);
        case HANTEK_DRC_DATA_TYPE_U64: return sizeof(uint64_t);
        case HANTEK_DRC_DATA_TYPE_U32: return sizeof(uint32_t);
        case HANTEK_DRC_DATA_TYPE_U16: return sizeof(uint16_t);
        case HANTEK_DRC_DATA_TYPE_U8:  return sizeof(uint8_t);
        case HANTEK_DRC_DATA_TYPE_F64: return sizeof(double_t);
        default:                       return sizeof(float_t);
    }
}

hantek_drc_data_value hantek_drc_data(hantek_drc_channel* channel, int16_t data) {
    hantek_drc_data_handler* handler = &channel->info->data_handler;
    if (handler->on_data != NULL) {
        return handler->on_data(channel, data);
    }
    hantek_drc_data_value value;
    switch (handler->type) {
    case HANTEK_DRC_DATA_TYPE_F32: value.f32 = (float_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_F64: value.f64 = (double_t) data; break;
    case HANTEK_DRC_DATA_TYPE_U8:  value.u8  = (uint8_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_U16: value.u16 = (uint16_t) data; break;
    case HANTEK_DRC_DATA_TYPE_U32: value.u32 = (uint32_t) data; break;
    case HANTEK_DRC_DATA_TYPE_U64: value.u64 = (uint64_t) data; break;
    case HANTEK_DRC_DATA_TYPE_I8:  value.i8  = (int8_t)   data; break;
    case HANTEK_DRC_DATA_TYPE_I16: value.i16 = (int16_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_I32: value.i32 = (int32_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_I64: value.i64 = (int64_t)  data; break;
    }
    return value;
}

void* hantek_drc_data_frame(hantek_drc_channel* channel, const int16_t* frame) {
    hantek_drc_data_handler* handler = &channel->info->data_handler;
    size_t sz = hantek_drc_data_type_size(handler->type);
    void* data = malloc(sz * channel->info->buffer_length);
    if (data == NULL) {
        return NULL;
    }
    for (size_t i=0; i < channel->info->buffer_length; ++i) {
        hantek_drc_data_value v = hantek_drc_data(channel, frame[i]);
        memcpy((unsigned char*)data + (sz * i), &v, sz);
    }
    return data;
}

hantek_drc_data_value hantek_drc_data_format(hantek_drc_channel* channel, int16_t data) {
    hantek_drc_data_handler* handler = &channel->info->data_handler;
    hantek_drc_data_format_payload* payload = (hantek_drc_data_format_payload*)channel->info->data_handler.payload;
    
    int64_t multiplier = payload->multiplier_fn != NULL ? payload->multiplier_fn(channel) : payload->multiplier;
    if (multiplier == 0) {
        multiplier = 1;
    }

    int64_t divider = payload->divider_fn != NULL ? payload->divider_fn(channel) : payload->divider;
    if (divider == 0) {
        divider = 1;
    }

    int64_t value = data * multiplier;
    
    hantek_drc_data_value result;
    if (handler->type == HANTEK_DRC_DATA_TYPE_F32) {
        float_t v = ((float_t) value) / divider;
        result.f32 = payload->positive && v < 0.0 ? 0.f : v;
    } else if (handler->type == HANTEK_DRC_DATA_TYPE_F64) {
        double_t v = ((double_t) value) / divider;
        result.f64 = payload->positive && v < 0.0 ? 0.0 : v;
    } else {
        value /= divider;
        if (payload->positive && value < 0) {
            value = 0;
        }
        switch (handler->type) {
        case HANTEK_DRC_DATA_TYPE_I32: result.i32 = (int32_t) value;    break;
        case HANTEK_DRC_DATA_TYPE_I16: result.i16 = (int16_t) value;    break;
        case HANTEK_DRC_DATA_TYPE_I8:  result.i8  = (int8_t) value;     break;
        case HANTEK_DRC_DATA_TYPE_U64: result.u64 = (uint64_t) value;   break;
        case HANTEK_DRC_DATA_TYPE_U32: result.u32 = (uint32_t) value;   break;
        case HANTEK_DRC_DATA_TYPE_U16: result.u16 = (uint16_t) value;   break;
        case HANTEK_DRC_DATA_TYPE_U8:  result.u8  = (uint8_t) value;    break;
        default:                       result.i64 = value;              break;
        }
    }
    return result;
}

bool hantek_drc_data_format_init(
    hantek_drc_info* info, 
    hantek_drc_data_type type, 
    hantek_drc_data_format_payload payload_example
) {
    hantek_drc_data_format_payload* payload = (hantek_drc_data_format_payload*) 
        calloc(1, sizeof(hantek_drc_data_format_payload));
    if (payload == NULL) {
        return false;
    }
    *payload = payload_example;
    info->data_handler = (hantek_drc_data_handler) {
        .type = type,
        .on_data = &hantek_drc_data_format,
        .on_free = &hantek_drc_data_format_free,
        .payload = payload
    };
    return true;
}

bool hantek_drc_data_format_raw(hantek_drc_info* info, hantek_drc_data_type type) {
    return hantek_drc_data_format_muldiv(info, type, 1, 1, false);
}

bool hantek_drc_data_format_volts_milli(hantek_drc_info* info, hantek_drc_data_type type) {
    return hantek_drc_data_format_init(info, type, (hantek_drc_data_format_payload) {
        .multiplier_fn = &hantek_drc_channel_max_volts_milli,
        .divider = INT16_MAX
    });
}

bool hantek_drc_data_format_volts(hantek_drc_info* info, hantek_drc_data_type type) {
    return hantek_drc_data_format_init(info, type, (hantek_drc_data_format_payload) {
        .multiplier_fn = &hantek_drc_channel_max_volts_milli,
        .divider = INT16_MAX * 1000ULL
    });
}

bool hantek_drc_data_format_muldiv(
    hantek_drc_info* info, hantek_drc_data_type type, bool positive, int64_t multiplier, int64_t divider
) {
    return hantek_drc_data_format_init(info, type, (hantek_drc_data_format_payload) {
        .divider = divider,
        .multiplier = multiplier,
        .positive = positive
    });
}

bool hantek_drc_data_format_mul(hantek_drc_info* info, hantek_drc_data_type type, bool positive, int64_t multiplier) {
    return hantek_drc_data_format_muldiv(info, type, positive, multiplier, INT16_MAX);
}

void hantek_drc_data_format_free(hantek_drc_info* info) {
    if (info->data_handler.payload != NULL) {
        free(info->data_handler.payload);
        info->data_handler.payload = NULL;
    }
}
