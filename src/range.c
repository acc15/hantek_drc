#include "range.h"

bool hantek_drc_range_contains(const hantek_drc_range* range, size_t value) {
    return !range->enabled || (value >= range->min && value <= range->max);
}
