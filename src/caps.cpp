#include "caps.hpp"

namespace hantek_drc {

caps caps::hantek_6254bd() {
    return caps {
        .max_channels = 4,
        .max_sampling_rate = 1000000000000ULL,
        .x_div = 10,
        .y_div = 8
    };
}

}