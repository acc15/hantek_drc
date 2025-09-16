#include "hantek_drc/caps.h"

// NOLINTBEGIN(readability-magic-numbers)
hantek_drc_caps hantek_drc_6254bd(void) {
    return (hantek_drc_caps) {
        .max_channels = 4,
        .max_sampling_rate = 1000000000000ULL,
        .x_div = 10,
        .y_div = 8
    };
}
// NOLINTEND(readability-magic-numbers)
