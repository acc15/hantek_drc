#include "mem.h"

#include <stdlib.h>
#include <string.h>

bool hantek_drc_mem_prepare(hantek_drc_info* info) {
    hantek_drc_mem_payload* payload = (hantek_drc_mem_payload*) info->frame_handler.payload;
    payload->data = calloc(info->channel_count, sizeof(void**));
    if (payload->data == NULL) {
        return false;
    }

    size_t frames_allocated = 16;
    for (size_t i = 0; i < info->channel_count; ++i) {
        payload->data[i] = calloc(frames_allocated, sizeof(void*));
        if (payload->data[i] == NULL) {
            return false;
        }
    }
    payload->frames_allocated = frames_allocated;
    return true;
}

bool hantek_drc_mem_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_info* info = channel->info;
    hantek_drc_mem_payload* payload = (hantek_drc_mem_payload*) info->frame_handler.payload;
    if (payload->frames_allocated <= info->frame_count) {
        size_t frames_allocated = payload->frames_allocated * 2;
        for (size_t i = 0; i < info->channel_count; ++i) {
            payload->data[i] = realloc(payload->data[i], sizeof(void**)*frames_allocated);
            if (payload->data[i] != NULL) {
                memset(payload->data[i] + payload->frames_allocated, 0, 
                    sizeof(float**)*(frames_allocated - payload->frames_allocated));
            } else {
                return false;
            }
        }
        payload->frames_allocated = frames_allocated;
    }
    void* frame_data = hantek_drc_data_frame(&channel->info->data_handler, channel, buffer);
    if (frame_data == NULL) {
        return false;
    }
    payload->data[channel->index][info->frame_count] = frame_data;
    return true;
}

void hantek_drc_mem_free(hantek_drc_info* info) {
    hantek_drc_mem_payload* payload = (hantek_drc_mem_payload*) info->frame_handler.payload;
    if (payload != NULL) {
        if (payload->data != NULL) {
            for (size_t i = 0; i < info->channel_count; ++i) {
                if (payload->data[i] != NULL) {
                    for (size_t j = 0; j < payload->frames_allocated; ++j) {
                        if (payload->data[i][j] != NULL) {
                            free(payload->data[i][j]);
                        }
                    }
                    free(payload->data[i]);
                }
            }
            free(payload->data);
        }
        free(payload);
        info->frame_handler.payload = NULL;
    }
}

bool hantek_drc_mem_init(hantek_drc_info* info) {
    hantek_drc_mem_payload* payload = calloc(1, sizeof(hantek_drc_mem_payload));
    if (payload == NULL) {
        return false;
    }
    info->frame_handler = (hantek_drc_frame_handler) {
        .on_prepare = &hantek_drc_mem_prepare,
        .on_frame = &hantek_drc_mem_frame,
        .on_free = &hantek_drc_mem_free,
        .payload = payload
    };
    return true;
}
