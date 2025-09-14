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

#ifdef __cplusplus
}
#endif
