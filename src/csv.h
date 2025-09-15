#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "info.h"
#include "format.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hantek_drc_csv_column {
    HANTEK_DRC_CSV_COLUMN_DEFAULT, // stub value to use default column list
    HANTEK_DRC_CSV_COLUMN_FRAME_INDEX, // sample index local to frame
    HANTEK_DRC_CSV_COLUMN_CHANNEL_INDEX, // sample index local to channel
    HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX, // global sample index (unique across all channels)
    HANTEK_DRC_CSV_COLUMN_FRAME, // frame index
    HANTEK_DRC_CSV_COLUMN_DATA, // data
    HANTEK_DRC_CSV_COLUMN_CHANNEL, // channel index
    HANTEK_DRC_CSV_COLUMN_CHANNEL_NUMBER, // channel number
} hantek_drc_csv_column;

extern const size_t HANTEK_DRC_CSV_COLUMN_BITS;

typedef struct hantek_drc_csv_params {
    
    const char* path;
    FILE* file;
    bool should_close;

    hantek_drc_csv_column columns;
    const char* column_separator;
    const char* line_separator;

    hantek_drc_format_handler format;

} hantek_drc_csv_params;

hantek_drc_csv_column hantek_drc_csv_columns(size_t count, ...);
hantek_drc_csv_column hantek_drc_csv_column_at(hantek_drc_csv_column cols, size_t index);

hantek_drc_frame_handler hantek_drc_csv(hantek_drc_csv_params* params);
HANTEK_DRC_HANDLER_ALLOC_DEF(csv, frame);

bool hantek_drc_csv_frame(void* params_any, const hantek_drc_channel* channel, const int16_t* buffer);
void hantek_drc_csv_free(void* params_any, const hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
