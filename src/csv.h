#pragma

#include <stdio.h>

#include "info.h"

typedef struct hantek_drc_csv_payload {
    FILE* csv_file;
} hantek_drc_csv_payload;

void hantek_drc_csv_append_data(hantek_drc_channel_info* channel, const int16_t* buffer, size_t frame_index);