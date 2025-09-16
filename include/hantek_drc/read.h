#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const size_t HANTEK_DRC_DATA_BUFFER_LENGTH;

size_t hantek_drc_header_length(hantek_drc_info* info);

bool hantek_drc_read_signature(FILE* file);
bool hantek_drc_read_channels(FILE* file, hantek_drc_info* info);
bool hantek_drc_read_general(FILE* file, hantek_drc_info* info);
bool hantek_drc_read_header(FILE* file, hantek_drc_info* info);
bool hantek_drc_read_frame(FILE* file, hantek_drc_info* info, int16_t* buffer);
bool hantek_drc_read_data(FILE* file, hantek_drc_info* info);
bool hantek_drc_read(FILE* file, hantek_drc_info* info);
bool hantek_drc_read_file(const char* path, hantek_drc_info* info);

#ifdef __cplusplus
}
#endif
