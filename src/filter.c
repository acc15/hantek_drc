#include "filter.h"

hantek_drc_frame_handler hantek_drc_filter(hantek_drc_filter_params* params) {
    return (hantek_drc_frame_handler) {
        .on_frame = &hantek_drc_filter_frame,
        .on_prepare = &hantek_drc_filter_prepare,
        .on_free = &hantek_drc_filter_free,
        .params = params
    };
}

HANTEK_DRC_HANDLER_ALLOC_IMPL(filter, frame)

bool hantek_drc_filter_prepare(void* params_any, const hantek_drc_info* info) {
    hantek_drc_filter_params* params = (hantek_drc_filter_params*) params_any;
    return hantek_drc_frame_handler_prepare(&params->filter, info) && 
        hantek_drc_frame_handler_prepare(&params->handler, info);
}

bool hantek_drc_filter_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_filter_params* params = (hantek_drc_filter_params*) params_any;
    return !hantek_drc_frame_handler_frame(&params->filter, channel, buffer) || 
        hantek_drc_frame_handler_frame(&params->handler, channel, buffer);
}

void hantek_drc_filter_free(void* params_any, const hantek_drc_info* info) {
    hantek_drc_filter_params* params = (hantek_drc_filter_params*) params_any;
    hantek_drc_handler_free(&params->filter, info);
    hantek_drc_handler_free(&params->handler, info);
}

hantek_drc_frame_handler hantek_drc_range_filter(hantek_drc_range_filter_params* params) {
    return (hantek_drc_frame_handler) {
        .on_frame = &hantek_drc_range_filter_frame,
        .params = params
    };
}

HANTEK_DRC_HANDLER_ALLOC_IMPL(range_filter, frame)

bool hantek_drc_range_filter_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* buffer) {
    (void)buffer;
    hantek_drc_range_filter_params* params = (hantek_drc_range_filter_params*) params_any;
    return hantek_drc_range_contains(&params->channel, channel->index) && 
        hantek_drc_range_contains(&params->frame, channel->info->frame_count);
}
