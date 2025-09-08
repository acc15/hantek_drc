#include "csv.h"
#include <stdlib.h>

bool hantek_drc_csv_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) channel->info->frame_handler.payload;
    FILE* file = payload->csv_file;
    size_t buffer_length = channel->info->buffer_length;
    for (size_t i = 0; i < buffer_length; ++i) {
        hantek_drc_data_value value = channel->info->data_handler.map(&channel->info->data_handler, channel, buffer[i]);
        int result = fprintf(file, "%zu\t%zu\t%zu\t%f\n", 
            channel->number + 1, channel->info->frame_count, i, value.f32);
        if (result < 0) {
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

bool hantek_drc_csv_init(hantek_drc_info* info, const char* path) {
    FILE* csv_file = fopen(path, "wt");
    if (csv_file == NULL) {
        return false;
    }

    hantek_drc_csv_payload* payload = (hantek_drc_csv_payload*) malloc(sizeof(hantek_drc_csv_payload));
    if (payload == NULL) {
        fclose(csv_file);
        return false;
    }

    payload->csv_file = csv_file;
    info->frame_handler = (hantek_drc_frame_handler) {
        .on_frame = &hantek_drc_csv_frame,
        .on_free = &hantek_drc_csv_free,
        .payload = payload
    };
    return true;
}
