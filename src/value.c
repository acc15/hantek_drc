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

uint64_t hantek_drc_voltage_volts_milli(uint16_t voltage, uint16_t multiplier) {
    return hantek_drc_value(voltage) * hantek_drc_fast_pow_10(multiplier);
}

uint64_t hantek_drc_timediv_nanos(uint16_t timediv) {
    return hantek_drc_value(timediv);
}

uint64_t hantek_drc_sampling_rate_milli(size_t x_div, uint64_t max_rate, uint64_t timediv_ns) {
    uint64_t computed_rate = 2500000000000000ULL / (x_div * timediv_ns);
    return computed_rate > max_rate ? max_rate : computed_rate;
}

uint64_t hantek_drc_max_volts_milli(size_t y_div, uint16_t voltage, uint16_t multiplier) {
    return hantek_drc_voltage_volts_milli(voltage, multiplier) * (y_div >> 1);
}
