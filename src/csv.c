#include "csv.h"
#include <stdlib.h>
#include <stdarg.h>

const size_t HANTEK_DRC_CSV_COLUMN_BITS = 3;

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_csv_params* params = (hantek_drc_csv_params*) channel->info->frame_handler.params;
    FILE* file = params->file;
    size_t buffer_length = channel->info->buffer_length;
    for (size_t i = 0; i < buffer_length; ++i) {
        
        for (size_t col_num = 0;; ++col_num) {
            hantek_drc_csv_column col = hantek_drc_csv_column_at(params->columns, col_num);
            if (col == HANTEK_DRC_CSV_COLUMN_DEFAULT) {
                break;
            }

            if (col_num > 0 && fputs(params->column_separator, file) == EOF) {
                return false;
            }

            int print_result = 0;
            switch (col) {
            case HANTEK_DRC_CSV_COLUMN_CHANNEL:
                print_result = fprintf(file, "%zu", channel->number + 1);
                break;

            case HANTEK_DRC_CSV_COLUMN_FRAME:
                print_result = fprintf(file, "%zu", channel->info->frame_count);
                break;

            case HANTEK_DRC_CSV_COLUMN_INDEX:
                print_result = fprintf(file, "%zu", i);
                break;

            case HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX:
                {
                    size_t global_index = (
                        ((channel->info->channel_count * channel->info->frame_count) + channel->index)
                        * buffer_length
                    ) + i;
                    print_result = fprintf(file, "%zu", global_index);
                    break;
                }

            default: 
                {
                    hantek_drc_data_value value = hantek_drc_format_data(&params->format, channel, buffer[i]);
                    switch (params->format.type) {
                    case HANTEK_DRC_DATA_TYPE_F32: print_result = fprintf(file, "%f", (double_t) value.f32); break;
                    case HANTEK_DRC_DATA_TYPE_F64: print_result = fprintf(file, "%f", value.f64); break;
                    case HANTEK_DRC_DATA_TYPE_U8:  print_result = fprintf(file, "%hhu", value.u8); break;
                    case HANTEK_DRC_DATA_TYPE_U16: print_result = fprintf(file, "%hu", value.u16); break;
                    case HANTEK_DRC_DATA_TYPE_U32: print_result = fprintf(file, "%u", value.u32); break;
                    case HANTEK_DRC_DATA_TYPE_U64: print_result = fprintf(file, "%lu", value.u64); break;
                    case HANTEK_DRC_DATA_TYPE_I8:  print_result = fprintf(file, "%hhd", value.i8); break;
                    case HANTEK_DRC_DATA_TYPE_I16: print_result = fprintf(file, "%hd", value.i16); break;
                    case HANTEK_DRC_DATA_TYPE_I32: print_result = fprintf(file, "%d", value.i32); break;
                    case HANTEK_DRC_DATA_TYPE_I64: print_result = fprintf(file, "%ld", value.i64); break;
                    }
                    break;
                }
            }
            if (print_result < 0) {
                return false;
            }
        }
        if (fputs(params->line_separator, file) == EOF) {
            return false;
        }
        
    }
    return true;
}

void hantek_drc_csv_free(hantek_drc_info* info) {
    hantek_drc_csv_params* params = (hantek_drc_csv_params*) info->frame_handler.params;
    if (params != NULL && params->file != NULL && params->should_close) {
        fclose(params->file);
        params->file = NULL;
    }
}

hantek_drc_frame_handler hantek_drc_csv_handler(hantek_drc_csv_params* params) {
    if (params->file == NULL) {
        if (params->path != NULL) {
            params->file = fopen(params->path, "wt");
            params->should_close = true;
        } else {
            return (hantek_drc_frame_handler) {0};
        }
    }

    if (params->file == NULL) {
        return (hantek_drc_frame_handler) {0};
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

    return (hantek_drc_frame_handler) {
        .on_prepare = NULL,
        .on_frame = &hantek_drc_csv_frame,
        .on_free = &hantek_drc_csv_free,
        .params = params
    };
}

HANTEK_DRC_HANDLER_ALLOC_IMPL(csv, frame)

hantek_drc_csv_column hantek_drc_csv_column_at(hantek_drc_csv_column cols, size_t index) {
    return (cols >> (HANTEK_DRC_CSV_COLUMN_BITS * index)) & ((1 << HANTEK_DRC_CSV_COLUMN_BITS) - 1);
}

hantek_drc_csv_column hantek_drc_csv_columns(size_t count, ...) {
    va_list args;
    va_start(args, count);
    hantek_drc_csv_column result = 0;
    for (size_t i = 0; i < count; ++i) {
        hantek_drc_csv_column col = va_arg(args, hantek_drc_csv_column);
        result |= (col << (HANTEK_DRC_CSV_COLUMN_BITS * i));
    }
    va_end(args);
    return result;
}
