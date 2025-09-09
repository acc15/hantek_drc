#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "info.h"
#include "data.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum hantek_drc_csv_payload_column {
    HANTEK_DRC_CSV_COLUMN_DEFAULT,
    HANTEK_DRC_CSV_COLUMN_INDEX,
    HANTEK_DRC_CSV_COLUMN_FRAME,
    HANTEK_DRC_CSV_COLUMN_DATA,
    HANTEK_DRC_CSV_COLUMN_CHANNEL,
    HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX
} hantek_drc_csv_payload_column;

typedef struct hantek_drc_csv_payload {
    FILE* csv_file;
    hantek_drc_csv_payload_column columns;
    const char* column_separator;
    const char* line_separator;
} hantek_drc_csv_payload;

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer);
bool hantek_drc_csv_init(hantek_drc_info* info, hantek_drc_csv_payload payload);
void hantek_drc_csv_free(hantek_drc_info* info);
hantek_drc_csv_payload_column hantek_drc_csv_columns(size_t count, ...);
#ifdef __cplusplus
}
#endif
