#include "info.hpp"

#include "file.hpp"
#include "binary.hpp"

namespace hantek_drc {

info::info(const hantek_drc::caps& caps): caps(caps) {}

std::istream& info::read_signature(std::istream& stream) {
    file_signature signature;
    if (stream >> binary_ref(signature) && !signature.is_valid()) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

std::istream& info::read_channels(std::istream& stream) {
    for (size_t i = 0; i < caps.max_channels; ++i) {
        std::uint32_t channel_enabled;
        if (!(stream >> binary_ref(channel_enabled))) {
            return stream;
        }
        if (!channel_enabled) {
            continue;
        }

        file_channel fc;
        if (!(stream >> binary_ref(fc))) {
            return stream;
        }

        channels.emplace_back(channel { 
            .info = *this, 
            .index = channels.size(),
            .number = i, 
            .voltage = fc.voltage,
            .multiplier = fc.multiplier,
            .coupling = static_cast<coupling>(fc.coupling),
            .lever = fc.lever_1
        });
    }
    return stream;
}

std::istream& info::read_general(std::istream& stream) {
    file_general general;
    if (!(stream >> binary_ref(general))) {
        return stream;
    }
    timediv = general.timediv;
    return stream;
}

std::istream& operator>>(std::istream& stream, info& info)  {
    info.read_signature(stream) && info.read_channels(stream) && info.read_general(stream);
    return stream;
}

}
