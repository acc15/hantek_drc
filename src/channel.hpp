#pragma once

#include <cstdint>
#include <cstddef>
#include <istream>

#include "coupling.hpp"

namespace hantek_drc {

class info;

class channel {
    std::size_t index; // zero-based index of enabled channel
    std::size_t number; // zero-based channel number (0 - 1CH, 1 - 2CH, etc)
    std::uint16_t voltage; // voltage index
    std::uint16_t multiplier; // multiplier index (most cases - 10^multiplier)
    enum coupling coupling;
    std::uint16_t lever; // signal y offset (lever) - values [0..255]
    class info* info;

public:
};


}
