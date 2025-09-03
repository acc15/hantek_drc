#pragma

#include <stdio.h>

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_csv_payload {
    FILE* csv_file;
} hantek_drc_csv_payload;

void hantek_drc_csv_append_data(hantek_drc_channel_info* channel, const int16_t* buffer);

#ifdef __cplusplus
}
#endif