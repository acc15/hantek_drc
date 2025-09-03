#include "caps.h"

void hantek_drc_init_6254bd(hantek_drc_device_capabilities* caps) {
    caps->max_channels = 4;
    caps->max_sampling_rate = 1000000000000ULL;
    caps->x_divisions = 10;
    caps->y_divisions = 8;
}