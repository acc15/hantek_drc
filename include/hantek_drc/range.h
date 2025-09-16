#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_range {
    bool enabled;
    size_t min;
    size_t max;
} hantek_drc_range;

bool hantek_drc_range_contains(const hantek_drc_range* range, size_t value);
hantek_drc_range hantek_drc_parse_range(const char* str);

#ifdef __cplusplus
}
#endif
