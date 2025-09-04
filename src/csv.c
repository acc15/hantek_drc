#include "csv.h"

#include <stdlib.h>

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) channel->info->payload;
    FILE* file = payload->csv_file;
    size_t buffer_length = channel->info->buffer_length;
    for (size_t i = 0; i < buffer_length; ++i) {
        int result = fprintf(file, "%zu\t%zu\t%zu\t%f\n", 
            channel->number + 1, channel->info->frame_count, i, payload->data_fn(channel, buffer[i]));
        if (result < 0) {
            return false;
        }
    }
    return true;
}

void hantek_drc_csv_free(hantek_drc_info* info) {
    if (info->payload != NULL) {
        hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) info->payload;
        if (payload->csv_file != NULL) {
            fclose(payload->csv_file);
        }
        free(info->payload);
        info->payload = NULL;
    }
}

bool hantek_drc_csv_init(hantek_drc_info* info, const char* path, hantek_drc_data_fn data_fn) {
    FILE* csv_file = fopen(path, "wt");
    if (csv_file == NULL) {
        return false;
    }

    hantek_drc_csv_payload* payload = malloc(sizeof(hantek_drc_csv_payload));
    if (payload == NULL) {
        fclose(csv_file);
        return false;
    }

    payload->csv_file = csv_file;
    payload->data_fn = hantek_drc_data_fn_or_default(data_fn);
    
    info->payload = payload;
    info->on_frame = &hantek_drc_csv_frame;
    info->on_free = &hantek_drc_csv_free;

    return true;
}
