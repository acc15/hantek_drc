#pragma once

#include <cstdint>
#include <cstddef>

#include "coupling.hpp"

namespace hantek_drc {

struct info;

struct channel {
    struct info& info;
    std::size_t index; // zero-based index of enabled channel
    std::size_t number; // one-based channel number (1 - 1CH, 2 - 2CH, etc)
    std::uint16_t voltage; // voltage index
    std::uint16_t multiplier; // multiplier index (most cases - 10^multiplier)
    enum coupling coupling;
    std::uint16_t lever; // signal y offset (lever) - values [0..255]
};

}
