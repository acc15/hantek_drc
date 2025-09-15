#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <istream>

#include "caps.hpp"
#include "channel.hpp"

namespace hantek_drc {

struct info {
    const struct caps caps;
    std::vector<channel> channels;
    uint16_t timediv; // timediv index
    size_t buffer_length; // buffer length (count of data points in single frame)
    size_t frame_count; // count of frames that has been read from files

    info(const hantek_drc::caps& caps);

    std::istream& read_signature(std::istream& stream);
    std::istream& read_channels(std::istream& stream);
    std::istream& read_general(std::istream& stream);

    friend std::istream& operator>>(std::istream& stream, info& info);
};

std::istream& operator>>(std::istream& stream, info& info);

}