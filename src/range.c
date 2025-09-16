#include "hantek_drc/range.h"

#include <stdlib.h>

bool hantek_drc_range_contains(const hantek_drc_range* range, size_t value) {
    return !range->enabled || (value >= range->min && value <= range->max);
}

hantek_drc_range hantek_drc_parse_range(const char* str) {
    hantek_drc_range range = {.enabled = false};
    if (str == NULL) {
        return range;
    }
    
    const char* start_ptr = str;
    char* end_ptr;

    range.min = range.max = strtol(start_ptr, &end_ptr, 10);
    if (end_ptr == start_ptr) {
        return range;
    }

    range.enabled = true;
    if (*end_ptr == 0) {
        return range;
    }

    if (*end_ptr == '-') {
        start_ptr = end_ptr + 1;
    }
    range.max = strtol(start_ptr, &end_ptr, 10);
    if (end_ptr == start_ptr) {
        range.max = range.min;
    }
    return range;
}
