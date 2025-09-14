#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "format.h"
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

int hantek_drc_data_print(FILE* file, hantek_drc_data_type type, hantek_drc_data_value value) {
    switch (type) {
    case HANTEK_DRC_DATA_TYPE_F32: return fprintf(file, "%f", (double_t) value.f32);
    case HANTEK_DRC_DATA_TYPE_F64: return fprintf(file, "%f", value.f64);
    case HANTEK_DRC_DATA_TYPE_U8:  return fprintf(file, "%hhu", value.u8);
    case HANTEK_DRC_DATA_TYPE_U16: return fprintf(file, "%hu", value.u16);
    case HANTEK_DRC_DATA_TYPE_U32: return fprintf(file, "%u", value.u32);
    case HANTEK_DRC_DATA_TYPE_U64: return fprintf(file, "%lu", value.u64);
    case HANTEK_DRC_DATA_TYPE_I8:  return fprintf(file, "%hhd", value.i8);
    case HANTEK_DRC_DATA_TYPE_I16: return fprintf(file, "%hd", value.i16);
    case HANTEK_DRC_DATA_TYPE_I32: return fprintf(file, "%d", value.i32);
    case HANTEK_DRC_DATA_TYPE_I64: return fprintf(file, "%ld", value.i64);
    default:
        errno = EINVAL;
        return -1;
    }
}

hantek_drc_data_type hantek_drc_format_type(
    hantek_drc_format_handler* handler, 
    const hantek_drc_channel* channel
) {
    return handler->on_type != NULL 
        ? handler->on_type(handler->params, channel)
        : HANTEK_DRC_DATA_TYPE_I16;
}

hantek_drc_data_value hantek_drc_format_data(
    hantek_drc_format_handler* handler, 
    const hantek_drc_channel* channel, 
    int16_t data
) {
    if (handler->on_data != NULL) {
        return handler->on_data(handler->params, channel, data);
    }
    hantek_drc_data_value value;
    hantek_drc_data_type type = hantek_drc_format_type(handler, channel);
    switch (type) {
    case HANTEK_DRC_DATA_TYPE_F32: value.f32 = (float_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_F64: value.f64 = (double_t) data; break;
    case HANTEK_DRC_DATA_TYPE_U8:  value.u8  = (uint8_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_U16: value.u16 = (uint16_t) data; break;
    case HANTEK_DRC_DATA_TYPE_U32: value.u32 = (uint32_t) data; break;
    case HANTEK_DRC_DATA_TYPE_U64: value.u64 = (uint64_t) data; break;
    case HANTEK_DRC_DATA_TYPE_I8:  value.i8  = (int8_t)   data; break;
    case HANTEK_DRC_DATA_TYPE_I16: value.i16 =            data; break;
    case HANTEK_DRC_DATA_TYPE_I32: value.i32 = (int32_t)  data; break;
    case HANTEK_DRC_DATA_TYPE_I64: value.i64 = (int64_t)  data; break;
    }
    return value;
}

void* hantek_drc_format_frame(
    hantek_drc_format_handler* handler, 
    const hantek_drc_channel* channel, 
    const int16_t* frame
) {
    size_t data_size = hantek_drc_data_type_size(hantek_drc_format_type(handler, channel));
    void* data = malloc(data_size * channel->info->buffer_length);
    if (data == NULL) {
        return NULL;
    }
    for (size_t i=0; i < channel->info->buffer_length; ++i) {
        hantek_drc_data_value data_value = hantek_drc_format_data(handler, channel, frame[i]);
        memcpy((unsigned char*)data + (data_size * i), &data_value, data_size);
    }
    return data;
}

hantek_drc_data_type hantek_drc_data_format_type(
    void* params_any, 
    const hantek_drc_channel* channel
) {
    (void)channel;
    hantek_drc_data_format_params* params = (hantek_drc_data_format_params*) params_any;
    return params->type; 
}

hantek_drc_data_value hantek_drc_data_format_data(
    void* params_any, 
    const hantek_drc_channel* channel, 
    int16_t data
) {
    hantek_drc_data_format_params* params = (hantek_drc_data_format_params*) params_any;
    
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
    if (params->type == HANTEK_DRC_DATA_TYPE_F32) {
        float_t float_value = (float_t) value / (float_t) divider;
        result.f32 = params->positive && float_value < 0.0 ? 0.F : float_value;
    } else if (params->type == HANTEK_DRC_DATA_TYPE_F64) {
        double_t double_value = (float_t) value / (float_t) divider;
        result.f64 = params->positive && double_value < 0.0 ? 0.0 : double_value;
    } else {
        value /= divider;
        if (params->positive && value < 0) {
            value = 0;
        }
        switch (params->type) {
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

hantek_drc_format_handler hantek_drc_data_format(hantek_drc_data_format_params* params) {
    return (hantek_drc_format_handler) {
        .on_type = &hantek_drc_data_format_type,
        .on_data = &hantek_drc_data_format_data,
        .params = params
    };
}

HANTEK_DRC_HANDLER_ALLOC_IMPL( data_format, format )

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
    static const uint64_t VOLTS_DIVIDER = 1000ULL;
    return (hantek_drc_data_format_params) {
        .type = type,
        .multiplier_fn = &hantek_drc_channel_max_volts_milli,
        .divider = INT16_MAX * VOLTS_DIVIDER
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
