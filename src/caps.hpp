#pragma once

#include <cstdint>
#include <cstddef>

namespace hantek_drc {

struct caps {
    std::size_t max_channels;
    std::uint64_t max_sampling_rate;
    std::size_t x_div;
    std::size_t y_div;

    static caps hantek_6254bd();
};

}
