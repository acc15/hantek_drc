#include "csv.h"
#include <stdlib.h>
#include <stdarg.h>

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_csv_params* params = (hantek_drc_csv_params*) channel->info->frame_handler.params;
    FILE* file = params->file;
    size_t buffer_length = channel->info->buffer_length;
    hantek_drc_csv_column cols = params->columns;
    for (size_t i = 0; i < buffer_length; ++i) {
        
        size_t col_num = 0;
        hantek_drc_csv_column remaining_cols = cols;
        while (remaining_cols != 0) {
            hantek_drc_csv_column col = remaining_cols & 0x07;

            int pr = 0;
            if (col_num > 0 && fputs(params->column_separator, file) == EOF) {
                return false;
            }

            switch (col) {
            case HANTEK_DRC_CSV_COLUMN_CHANNEL:
                pr = fprintf(file, "%zu", channel->number + 1);
                break;

            case HANTEK_DRC_CSV_COLUMN_FRAME:
                pr = fprintf(file, "%zu", channel->info->frame_count);
                break;

            case HANTEK_DRC_CSV_COLUMN_INDEX:
                pr = fprintf(file, "%zu", i);
                break;

            case HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX:
                pr = fprintf(file, "%zu", 
                    (channel->info->channel_count * channel->info->frame_count + channel->index)*buffer_length + i);
                break;
            
            default: 
                {
                    hantek_drc_data_value value = hantek_drc_data(channel, buffer[i]);
                    switch (channel->info->data_handler.type) {
                    case HANTEK_DRC_DATA_TYPE_F32: pr = fprintf(file, "%f", (double_t) value.f32); break;
                    case HANTEK_DRC_DATA_TYPE_F64: pr = fprintf(file, "%f", value.f64); break;
                    case HANTEK_DRC_DATA_TYPE_U8:  pr = fprintf(file, "%hhu", value.u8); break;
                    case HANTEK_DRC_DATA_TYPE_U16: pr = fprintf(file, "%hu", value.u16); break;
                    case HANTEK_DRC_DATA_TYPE_U32: pr = fprintf(file, "%u", value.u32); break;
                    case HANTEK_DRC_DATA_TYPE_U64: pr = fprintf(file, "%lu", value.u64); break;
                    case HANTEK_DRC_DATA_TYPE_I8:  pr = fprintf(file, "%hhd", value.i8); break;
                    case HANTEK_DRC_DATA_TYPE_I16: pr = fprintf(file, "%hd", value.i16); break;
                    case HANTEK_DRC_DATA_TYPE_I32: pr = fprintf(file, "%d", value.i32); break;
                    case HANTEK_DRC_DATA_TYPE_I64: pr = fprintf(file, "%ld", value.i64); break;
                    }
                    break;
                }
            }

            if (pr < 0) {
                return false;
            }

            remaining_cols >>= 3;
            ++col_num;
        }
        if (fputs(params->line_separator, file) == EOF) {
            return false;
        }
        
    }
    return true;
}

void hantek_drc_csv_free(hantek_drc_info* info) {
    hantek_drc_frame_handler* fh = &info->frame_handler;
    if (fh->params != NULL) {
        hantek_drc_csv_params* params = (hantek_drc_csv_params*) fh->params;
        if (params->file != NULL && params->should_close) {
            fclose(params->file);
        }
        if (params->should_free) {
            free(params);
        }
        fh->params = NULL;
    }
}

bool hantek_drc_csv_params_ext(hantek_drc_info* info, hantek_drc_csv_params* params) {
    if (params->file == NULL) {
        if (params->path != NULL) {
            params->file = fopen(params->path, "wt");
            params->should_close = true;
        } else {
            return false;
        }
    }
    
    if (params->file == NULL) {
        return false;
    }

    if (params->columns == HANTEK_DRC_CSV_COLUMN_DEFAULT) {
        params->columns = hantek_drc_csv_columns(4, 
            HANTEK_DRC_CSV_COLUMN_CHANNEL, 
            HANTEK_DRC_CSV_COLUMN_FRAME, 
            HANTEK_DRC_CSV_COLUMN_INDEX,
            HANTEK_DRC_CSV_COLUMN_DATA
        );
    }

    if (params->column_separator == NULL) {
        params->column_separator = "\t";
    }

    if (params->line_separator == NULL) {
        params->line_separator = "\n";
    }

    info->frame_handler = (hantek_drc_frame_handler) {
        .on_frame = &hantek_drc_csv_frame,
        .on_free = &hantek_drc_csv_free,
        .params = params
    };
    return true;
}

bool hantek_drc_csv_params_alloc(hantek_drc_info* info, hantek_drc_csv_params params_example) {
    hantek_drc_csv_params* params = (hantek_drc_csv_params*) calloc(1, sizeof(hantek_drc_csv_params));
    if (params == NULL) {
        return false;
    }

    *params = params_example;
    params->should_free = true;
    if (!hantek_drc_csv_params_ext(info, params)) {
        free(params);
        return false;
    }
    return true;
}

hantek_drc_csv_column hantek_drc_csv_columns(size_t count, ...) {
    va_list args;
    va_start(args, count);
    hantek_drc_csv_column result = 0;
    for (size_t i = 0; i < count; ++i) {
        hantek_drc_csv_column col = va_arg(args, hantek_drc_csv_column);
        result |= (col << 3 * i);
    }
    va_end(args);
    return result;
}
