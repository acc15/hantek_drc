#include <memory.h>

#include "info.h"
#include "value.h"


uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info) {
    return hantek_drc_timediv_nanos(info->timediv);
}

uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info) {
    uint64_t timediv_nanos = hantek_drc_timediv_nanos(info->timediv);
    return hantek_drc_sampling_rate_milli(info->caps.x_div, info->caps.max_sampling_rate, timediv_nanos);
}

void hantek_drc_info_free(hantek_drc_info* info) {
    hantek_drc_handler_free(info, &info->frame_handler);
}
