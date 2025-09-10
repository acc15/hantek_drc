#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hantek_drc_csv_column {
    HANTEK_DRC_CSV_COLUMN_DEFAULT,
    HANTEK_DRC_CSV_COLUMN_INDEX,
    HANTEK_DRC_CSV_COLUMN_FRAME,
    HANTEK_DRC_CSV_COLUMN_DATA,
    HANTEK_DRC_CSV_COLUMN_CHANNEL,
    HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX
} hantek_drc_csv_column;

extern const size_t HANTEK_DRC_CSV_COLUMN_BITS;

typedef struct hantek_drc_csv_params {
    const char* path;
    FILE* file;
    hantek_drc_csv_column columns;
    const char* column_separator;
    const char* line_separator;
    bool should_close;
} hantek_drc_csv_params;

hantek_drc_frame_handler hantek_drc_csv_handler(hantek_drc_csv_params* params);
hantek_drc_csv_column hantek_drc_csv_columns(size_t count, ...);
hantek_drc_csv_column hantek_drc_csv_column_at(hantek_drc_csv_column cols, size_t index);

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer);
void hantek_drc_csv_free(hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
