#include "value.hpp"
#include "pow.hpp"

namespace hantek_drc {

std::uint64_t drc_value(std::uint16_t index) {
    return (10 >> (2 - (index % 3))) * ipow_lookup<10>(index / 3);
}

std::uint64_t voltage_millivolts(std::uint16_t voltage, std::uint16_t multiplier) {
    return drc_value(voltage) * ipow_lookup<10>(multiplier);
}

std::uint64_t timediv_nanos(std::uint16_t timediv) {
    return drc_value(timediv);
}

std::uint64_t sampling_rate_milli(std::size_t x_div, std::uint64_t max_rate, std::uint64_t timediv_nanos) {
    uint64_t computed_rate = 2500000000000000ULL / (x_div * timediv_nanos);
    return computed_rate > max_rate ? max_rate : computed_rate;
}

std::uint64_t max_millivolts(std::size_t y_div, std::uint16_t voltage, std::uint16_t multiplier) {
    return voltage_millivolts(voltage, multiplier) * (y_div / 2);
}

}