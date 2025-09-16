#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_caps {
    size_t max_channels;
    uint64_t max_sampling_rate;
    size_t x_div;
    size_t y_div;
} hantek_drc_caps;

hantek_drc_caps hantek_drc_6254bd(void);

#ifdef __cplusplus
}
#endif
