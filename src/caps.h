#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct hantek_drc_device_capabilities {
    size_t max_channels;
    uint64_t max_sampling_rate;
    size_t x_divisions;
    size_t y_divisions;
} hantek_drc_device_capabilities;

void hantek_drc_init_6254bd(hantek_drc_device_capabilities* caps);