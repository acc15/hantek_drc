#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "caps.hpp"
#include "channel.hpp"

namespace hantek_drc {

class info {
    caps caps;
    std::vector<channel> channels;
    uint16_t timediv; // timediv index
    size_t buffer_length; // buffer length (count of data points in single frame)
    size_t frame_count; // count of frames that has been read from files

};

}