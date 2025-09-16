#include <memory.h>

#include "hantek_drc/info.h"
#include "hantek_drc/value.h"


uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info) {
    return hantek_drc_timediv_nanos(info->timediv);
}

uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info) {
    uint64_t timediv_nanos = hantek_drc_timediv_nanos(info->timediv);
    return hantek_drc_sampling_rate_milli(info->caps.x_div, info->caps.max_sampling_rate, timediv_nanos);
}

bool hantek_drc_info_finish(hantek_drc_info* info) {
    return hantek_drc_frame_handler_finish(&info->handler, info);
}

void hantek_drc_info_free(hantek_drc_info* info) {
    hantek_drc_handler_free(&info->handler, info);
}

bool hantek_drc_info_finish_free(hantek_drc_info* info) {
    bool result = hantek_drc_info_finish(info);
    hantek_drc_info_free(info);
    return result;
}
