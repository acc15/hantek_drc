#include "value.h"

uint64_t hantek_drc_fast_pow_10(unsigned int pow) {
    static const uint64_t pow10[] = {
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL,
        1000000000000ULL,
        10000000000000ULL,
        100000000000000ULL,
        1000000000000000ULL,
        10000000000000000ULL,
        100000000000000000ULL,
        1000000000000000000ULL,
        10000000000000000000ULL
    };
    return pow < sizeof(pow10)/sizeof(pow10[0]) ? pow10[pow] : 1;
}

uint64_t hantek_drc_value(unsigned int index) {
    // sequence is 2, 5, 10
    // reversed sequence (2 - i) = 10, 5, 2
    // 10 == 1010b after >> 1 == 
    // 5 == 101b after >> 1 ==
    // 2 == 10b
    return (10 >> (2 - (index % 3))) * hantek_drc_fast_pow_10(index / 3);
}

uint64_t hantek_drc_voltage_millivolts(uint16_t voltage, uint16_t multiplier) {
    return hantek_drc_value(voltage) * hantek_drc_fast_pow_10(multiplier);
}

float hantek_drc_voltage_volts(uint16_t voltage, uint16_t multiplier) {
    return hantek_drc_voltage_millivolts(voltage, multiplier) / 1000.f;
}

uint64_t hantek_drc_timediv_nanos(uint16_t timediv) {
    return hantek_drc_value(timediv);
}

float hantek_drc_timediv_seconds(uint16_t timediv) {
    return hantek_drc_timediv_nanos(timediv) / 1000000000.f;
}

uint64_t hantek_drc_sampling_rate_milli(size_t x_div, uint64_t max_rate, uint64_t timediv_ns) {
    uint64_t computed_rate = 2500000000000000ULL / (x_div * timediv_ns);
    return computed_rate > max_rate ? max_rate : computed_rate;
}

float hantek_drc_sampling_rate(size_t x_div, uint64_t max_rate, uint64_t timediv_ns) {
    return hantek_drc_sampling_rate_milli(x_div, max_rate, timediv_ns) / 1000.f;
}

int64_t hantek_drc_max_millivolts(size_t y_div, uint16_t voltage, uint16_t multiplier) {
    return hantek_drc_voltage_millivolts(voltage, multiplier) * (y_div >> 1);
}

float hantek_drc_data_millivolts_multiplier(size_t y_div, uint16_t voltage, uint16_t multiplier) {
    return (float) hantek_drc_max_millivolts(y_div, voltage, multiplier) / INT16_MAX;
}

float hantek_drc_data_volts_multiplier(size_t y_div, uint16_t voltage, uint16_t multiplier) {
    return hantek_drc_data_millivolts_multiplier(y_div, voltage, multiplier) / 1000.f;
}

float hantek_drc_data_volts(size_t y_div, uint16_t voltage, uint16_t multiplier, int16_t raw_data) {
    return hantek_drc_data_volts_multiplier(y_div, voltage, multiplier) * raw_data;
}