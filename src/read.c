#include "read.h"
#include <string.h>

const uint8_t HANTEK_DRC_SIGNATURE[] = {
    0x00, 0x00, 0x02, 0x00, 0x14, 0x01, 0xa8, 0xc0, 0x00, 0xff, 0xff, 0xff, 0x01, 0x01, 0xa8, 0xc0, 0x50, 0xc3
};

#pragma pack(push, 1)
typedef struct hantek_drc_file_signature {
    uint8_t signature[sizeof(HANTEK_DRC_SIGNATURE)]; // 18
    uint16_t unknown_data_0; // 20
} hantek_drc_file_signature;

typedef struct hantek_drc_file_channel_info {
    uint16_t voltage; // 2
    uint16_t coupling; // 4
    uint32_t multiplier; // 8
    uint32_t unknown_pad_1; // 12
    uint16_t unknown_pad_2; // 14
    uint16_t lever_1; // 16
    uint16_t lever_2; // 18
    uint32_t unknown_pad_3; // 22
    uint32_t unknown_pad_4; // 26
} hantek_drc_file_channel_info;

typedef struct hantek_drc_file_general_info {
    uint32_t unknown_pad_1; // 4
    uint32_t unknown_pad_2; // 8
    uint32_t unknown_pad_3; // 12
    uint32_t unknown_pad_4; // 16
    uint32_t unknown_pad_5; // 20
    uint16_t unknown_pad_6; // 22
    uint16_t timediv; // 24
    uint16_t unknown_pad_7; // 26
    uint32_t unknown_pad_8[19]; // 102
} hantek_drc_file_general_info;

typedef struct hantek_drc_file_data_block_header {
    uint32_t unknown_pad_1; // 4
    uint16_t unknown_pad_2; // 6
    uint8_t unknown_pad_3; // 7
    uint8_t timediv; // 8
} hantek_drc_file_data_block_header;
#pragma pack(pop)

const size_t HANTEK_DRC_DATA_BUFFER_LENGTH = 4096;

bool hantek_drc_read_signature(FILE* file) {
    hantek_drc_file_signature header;
    size_t data_read = fread(&header, sizeof(hantek_drc_file_signature), 1, file);
    return data_read == 1 && memcmp(header.signature, HANTEK_DRC_SIGNATURE, sizeof(HANTEK_DRC_SIGNATURE)) == 0;
}

bool hantek_drc_read_channels(FILE* file, hantek_drc_info* info) {
    info->channel_count = 0;
    for (size_t i = 0; i < info->max_channels; ++i) {
        uint32_t enabled = 0;
        if (fread(&enabled, sizeof(enabled), 1, file) != 1) {
            return false;
        }
        if (enabled == 0) {
            continue;
        }
        hantek_drc_file_channel_info file_channel_info;
        if (fread(&file_channel_info, sizeof(hantek_drc_file_channel_info), 1, file) != 1) {
            return false;
        }

        hantek_drc_channel* ch = &info->channel[info->channel_count];
        ch->index = info->channel_count;
        ch->number = i;
        ch->coupling = file_channel_info.coupling;
        ch->multiplier = file_channel_info.multiplier;
        ch->lever = file_channel_info.lever_1;
        ch->voltage = file_channel_info.voltage;
        ch->info = info;
        ++info->channel_count;
    }
    return true;
}

bool hantek_drc_read_general(FILE* file, hantek_drc_info* info) {
    hantek_drc_file_general_info file_general;
    if (fread(&file_general, sizeof(hantek_drc_file_general_info), 1, file) != 1) {
        return false;
    }
    info->timediv = file_general.timediv;
    info->buffer_length = HANTEK_DRC_DATA_BUFFER_LENGTH;
    return true;
}

bool hantek_drc_read_header(FILE* file, hantek_drc_info* info) {
    if (!hantek_drc_read_signature(file)) {
        return false;
    }
    if (!hantek_drc_read_channels(file, info)) {
        return false;
    }
    return hantek_drc_read_general(file, info);
}

bool hantek_drc_read_data_frame(FILE* file, hantek_drc_channel* channel_info, int16_t* buffer) {
    hantek_drc_file_data_block_header block_header;
    if (fread(&block_header, sizeof(hantek_drc_file_data_block_header), 1, file) != 1) {
        return false;
    }
    size_t buffer_length = channel_info->info->buffer_length;
    return fread(buffer, sizeof(int16_t), buffer_length, file) == buffer_length;
}

bool hantek_drc_read_data(FILE* file, hantek_drc_info* info) {
    if (info->channel_count == 0) {
        return true;
    }
    size_t channel_index = 0;
    int16_t buffer[HANTEK_DRC_DATA_BUFFER_LENGTH];
    while (true) {
        hantek_drc_channel* channel = &info->channel[channel_index];
        bool has_data = hantek_drc_read_data_frame(file, channel, buffer);
        if (!has_data) {
            break;
        }
        if (info->on_frame != NULL && !info->on_frame(channel, buffer)) {
            return false;
        }
        ++channel_index;
        if (channel_index >= info->channel_count) {
            channel_index = 0;
            ++info->frame_count;
        }
    }
    return feof(file);
}

bool hantek_drc_read(FILE* file, hantek_drc_info* info) {
    if (!hantek_drc_read_header(file, info)) {
        return false;
    }
    if (info->frame_count == 0 && info->on_prepare != NULL && !info->on_prepare(info)) {
        return false;
    }
    return hantek_drc_read_data(file, info);
}

bool hantek_drc_read_file(const char* path, hantek_drc_info* info) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return false;
    }
    bool result = hantek_drc_read(file, info);
    fclose(file);
    return result;
}