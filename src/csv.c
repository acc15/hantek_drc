#include "csv.h"
#include <stdlib.h>
#include <stdarg.h>

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) channel->info->frame_handler.payload;
    FILE* file = payload->csv_file;
    size_t buffer_length = channel->info->buffer_length;
    hantek_drc_csv_payload_column cols = payload->columns;
    for (size_t i = 0; i < buffer_length; ++i) {
        
        size_t col_num = 0;
        hantek_drc_csv_payload_column remaining_cols = cols;
        while (remaining_cols != 0) {
            hantek_drc_csv_payload_column col = remaining_cols & 0x07;

            int pr = 0;
            if (col_num > 0 && fputs(payload->column_separator, file) == EOF) {
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
        if (fputs(payload->line_separator, file) == EOF) {
            return false;
        }
        
    }
    return true;
}

void hantek_drc_csv_free(hantek_drc_info* info) {
    hantek_drc_frame_handler* fh = (hantek_drc_frame_handler*) &info->frame_handler;
    if (fh->payload != NULL) {
        hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) fh->payload;
        if (payload->csv_file != NULL) {
            fclose(payload->csv_file);
        }
        free(payload);
        fh->payload = NULL;
    }
}

bool hantek_drc_csv_init(hantek_drc_info* info, hantek_drc_csv_payload payload_example) {
    if (payload_example.csv_file == NULL) {
        return false;
    }

    hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) calloc(1, sizeof(hantek_drc_csv_payload));
    if (payload == NULL) {
        return false;
    }

    *payload = payload_example;
    if (payload->columns == HANTEK_DRC_CSV_COLUMN_DEFAULT) {
        payload->columns = hantek_drc_csv_columns(4, 
            HANTEK_DRC_CSV_COLUMN_CHANNEL, 
            HANTEK_DRC_CSV_COLUMN_FRAME, 
            HANTEK_DRC_CSV_COLUMN_INDEX,
            HANTEK_DRC_CSV_COLUMN_DATA
        );
    }
    if (payload->column_separator == NULL) {
        payload->column_separator = "\t";
    }
    if (payload->line_separator == NULL) {
        payload->line_separator = "\n";
    }
    info->frame_handler = (hantek_drc_frame_handler) {
        .on_frame = &hantek_drc_csv_frame,
        .on_free = &hantek_drc_csv_free,
        .payload = payload
    };
    return true;
}

hantek_drc_csv_payload_column hantek_drc_csv_columns(size_t count, ...) {
    va_list args;
    va_start(args, count);
    hantek_drc_csv_payload_column result = 0;
    for (size_t i = 0; i < count; ++i) {
        hantek_drc_csv_payload_column col = va_arg(args, hantek_drc_csv_payload_column);
        result |= (col << 3 * i);
    }
    va_end(args);
    return result;
}
