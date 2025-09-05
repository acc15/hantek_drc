#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t hantek_drc_fast_pow_10(unsigned int pow);
uint64_t hantek_drc_value(unsigned int index);

uint64_t hantek_drc_voltage_millivolts(uint16_t voltage, uint16_t multiplier);
float hantek_drc_voltage_volts(uint16_t voltage, uint16_t multiplier);

uint64_t hantek_drc_timediv_nanos(uint16_t timediv);
float hantek_drc_timediv_seconds(uint16_t timediv);

uint64_t hantek_drc_sampling_rate_milli(size_t x_div, uint64_t max_rate, uint64_t time_div_ns);
float hantek_drc_sampling_rate(size_t x_div, uint64_t max_rate, uint64_t time_div_ns);

uint64_t hantek_drc_max_millivolts(size_t y_div, uint16_t voltage, uint16_t multiplier) ;
float hantek_drc_data_volts_multiplier(size_t y_div, uint16_t voltage, uint16_t multiplier);
float hantek_drc_data_volts(size_t y_div, uint16_t voltage, uint16_t multiplier, int16_t raw_data);

#ifdef __cplusplus
}
#endif
