#include "csv.h"

void hantek_drc_csv_append_data(hantek_drc_channel_info* channel, const int16_t* buffer) {
    hantek_drc_csv_payload* csv_payload = (hantek_drc_csv_payload*) channel->info->payload;
    FILE* file = csv_payload->csv_file;
    size_t buffer_length = channel->info->buffer_length;
    for (size_t i = 0; i < buffer_length; ++i) {
        fprintf(file, "%d\t%d\t%f\n", 
            channel->number + 1, 
            channel->info->frame_count, 
            hantek_drc_channel_data_volts(channel, buffer[i]) 
        );
    }
}