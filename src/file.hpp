#pragma once

#include <cstdint>
#include <array>
#include <istream>

namespace hantek_drc {

#pragma pack(push, 1)

struct file_signature {
    static constexpr std::array VALID_SIGNATURE = std::to_array<std::uint8_t>({
        0x00, 0x00, 0x02, 0x00, 0x14, 0x01, 0xa8, 0xc0, 0x00, 0xff, 0xff, 0xff, 0x01, 0x01, 0xa8, 0xc0, 0x50, 0xc3
    });

    std::array<std::uint8_t, VALID_SIGNATURE.size()> signature;
    std::uint16_t unknown_0; // 20
    bool is_valid() const;
};
std::istream& operator>>(std::istream& stream, file_signature&& signature);

struct file_channel {
    std::uint16_t voltage; // 2
    std::uint16_t coupling; // 4
    std::uint32_t multiplier; // 8
    std::uint32_t unknown_0; // 12
    std::uint16_t unknown_1; // 14
    std::uint16_t lever_1; // 16
    std::uint16_t lever_2; // 18
    std::uint32_t unknown_2; // 22
    std::uint32_t unknown_3; // 26
};

struct file_general {
    uint32_t unknown_0; // 4
    uint32_t unknown_1; // 8
    uint32_t unknown_2; // 12
    uint32_t unknown_3; // 16
    uint32_t unknown_4; // 20
    uint16_t unknown_5; // 22
    uint16_t timediv; // 24
    uint16_t unknown_6; // 26
    uint32_t unknown_7[19]; // 102
};

struct file_data_header {
    uint32_t unknown_0; // 4
    uint16_t unknown_1; // 6
    uint8_t unknown_2; // 7
    uint8_t timediv; // 8
};

#pragma pack(pop)

}