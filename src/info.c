#include <memory.h>

#include "info.h"
#include "value.h"

// NOLINTBEGIN(readability-magic-numbers)
struct hantek_drc_info hantek_drc_init_6254bd(void) {
    return (hantek_drc_info) {
        .max_channels = 4,
        .max_sampling_rate = 1000000000000ULL,
        .x_div = 10,
        .y_div = 8
    };
}
// NOLINTEND(readability-magic-numbers)

void hantek_drc_free( hantek_drc_info* info) {
    hantek_drc_handler_free(info, (hantek_drc_handler*) &info->frame_handler);
    hantek_drc_handler_free(info, (hantek_drc_handler*) &info->data_handler);
}

uint64_t hantek_drc_info_timediv_nanos(const hantek_drc_info* info) {
    return hantek_drc_timediv_nanos(info->timediv);
}

uint64_t hantek_drc_info_sampling_rate_milli(const hantek_drc_info* info) {
    uint64_t timediv_nanos = hantek_drc_timediv_nanos(info->timediv);
    return hantek_drc_sampling_rate_milli(info->x_div, info->max_sampling_rate, timediv_nanos);
}
