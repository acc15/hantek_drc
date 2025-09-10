#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t hantek_drc_fast_pow_10(unsigned int pow);
uint64_t hantek_drc_value(unsigned int index);

uint64_t hantek_drc_voltage_volts_milli(uint16_t voltage, uint16_t multiplier);
uint64_t hantek_drc_timediv_nanos(uint16_t timediv);
uint64_t hantek_drc_sampling_rate_milli(size_t x_div, uint64_t max_rate, uint64_t timediv_ns);
uint64_t hantek_drc_max_volts_milli(size_t y_div, uint16_t voltage, uint16_t multiplier);

#ifdef __cplusplus
}
#endif
