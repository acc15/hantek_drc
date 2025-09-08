#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "info.h"
#include "data.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_csv_payload {
    FILE* csv_file;
    hantek_drc_data_fn data_fn;
} hantek_drc_csv_payload;

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer);
bool hantek_drc_csv_init(hantek_drc_info* info, const char* path, hantek_drc_data_fn data_fn);

#ifdef __cplusplus
}
#endif
