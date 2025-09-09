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
    hantek_drc_data_format_params* params = (hantek_drc_data_format_params*)channel->info->data_handler.params;
    
    int64_t multiplier = params->multiplier_fn != NULL ? params->multiplier_fn(channel) : params->multiplier;
    if (multiplier == 0) {
        multiplier = 1;
    }

    int64_t divider = params->divider_fn != NULL ? params->divider_fn(channel) : params->divider;
    if (divider == 0) {
        divider = 1;
    }

    int64_t value = data * multiplier;
    
    hantek_drc_data_value result;
    if (handler->type == HANTEK_DRC_DATA_TYPE_F32) {
        float_t v = ((float_t) value) / divider;
        result.f32 = params->positive && v < 0.0 ? 0.f : v;
    } else if (handler->type == HANTEK_DRC_DATA_TYPE_F64) {
        double_t v = ((double_t) value) / divider;
        result.f64 = params->positive && v < 0.0 ? 0.0 : v;
    } else {
        value /= divider;
        if (params->positive && value < 0) {
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

bool hantek_drc_data_format_alloc(hantek_drc_info* info, hantek_drc_data_format_params params_example) {
    hantek_drc_data_format_params* params = (hantek_drc_data_format_params*) 
        calloc(1, sizeof(hantek_drc_data_format_params));
    if (params == NULL) {
        return false;
    }
    *params = params_example;
    if (!hantek_drc_data_format_ext(info, params)) {
        free(params);
        return false;
    }
    info->data_handler.should_free = true;
    return true;
}

bool hantek_drc_data_format_ext(hantek_drc_info* info, hantek_drc_data_format_params* params) {
    info->data_handler.type = params->type;
    info->data_handler.on_data = &hantek_drc_data_format;
    info->data_handler.on_free = NULL;
    info->data_handler.params = params;
    return true;
}

hantek_drc_data_format_params hantek_drc_data_format_raw(hantek_drc_data_type type) {
    return hantek_drc_data_format_muldiv(type, 1, 1, false);
}

hantek_drc_data_format_params hantek_drc_data_format_volts_milli(hantek_drc_data_type type) {
    return (hantek_drc_data_format_params) {
        .type = type,
        .multiplier_fn = &hantek_drc_channel_max_volts_milli,
        .divider = INT16_MAX
    };
}

hantek_drc_data_format_params hantek_drc_data_format_volts(hantek_drc_data_type type) {
    return (hantek_drc_data_format_params) {
        .type = type,
        .multiplier_fn = &hantek_drc_channel_max_volts_milli,
        .divider = INT16_MAX * 1000ULL
    };
}

hantek_drc_data_format_params hantek_drc_data_format_muldiv(
    hantek_drc_data_type type, 
    bool positive, 
    int64_t multiplier, 
    int64_t divider
) {
    return (hantek_drc_data_format_params) {
        .type = type,
        .divider = divider,
        .multiplier = multiplier,
        .positive = positive
    };
}

hantek_drc_data_format_params hantek_drc_data_format_mul(hantek_drc_data_type type, bool positive, int64_t multiplier) {
    return hantek_drc_data_format_muldiv(type, positive, multiplier, INT16_MAX);
}
