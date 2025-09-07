#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace hantek_drc {

std::uint64_t drc_value(std::uint16_t index);
std::uint64_t voltage_millivolts(std::uint16_t voltage, std::uint16_t multiplier);
std::uint64_t timediv_nanos(std::uint16_t timediv);
std::uint64_t sampling_rate_milli(std::size_t x_div, std::uint64_t max_rate, std::uint64_t time_div_nanos);
std::uint64_t max_millivolts(std::size_t y_div, std::uint16_t voltage, std::uint16_t multiplier);

template <typename T>
T voltage_volts(std::uint16_t voltage, std::uint16_t multiplier) {
    return voltage_millivolts(voltage, multiplier) / static_cast<T>(1000);
}

template <typename T>
T timediv_seconds(std::uint16_t timediv) {
    return timediv_nanos(timediv) / static_cast<T>(1000000000UL);
}

template <typename T>
T sampling_rate(std::size_t x_div, std::uint64_t max_rate, std::uint64_t timediv_nanos) {
    return sampling_rate_milli(x_div, max_rate, timediv_nanos) / static_cast<T>(1000);
}

template <typename T>
T data_millivolts_multiplier(size_t y_div, uint16_t voltage, uint16_t multiplier) {
    return max_millivolts(y_div, voltage, multiplier) / std::numeric_limits<uint16_t>::max();
}

template <typename T>
T data_volts_multiplier(size_t y_div, uint16_t voltage, uint16_t multiplier) {
    return data_millivolts_multiplier<T>(y_div, voltage, multiplier) / static_cast<T>(1000);
}

template <typename T>
T data_volts(size_t y_div, uint16_t voltage, uint16_t multiplier, int16_t raw_data) {
    return data_volts_multiplier<T>(y_div, voltage, multiplier) * static_cast<T>(raw_data);
}

}