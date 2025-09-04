#include "mem.h"

#include <stdlib.h>
#include <string.h>

bool hantek_drc_mem_prepare(hantek_drc_info* info) {
    hantek_drc_mem_payload* payload = (hantek_drc_mem_payload*) info->payload;
    payload->data = calloc(info->channel_count, sizeof(float**));
    if (payload->data == NULL) {
        return false;
    }

    size_t frames_allocated = 16;
    for (size_t i = 0; i < info->channel_count; ++i) {
        payload->data[i] = calloc(frames_allocated, sizeof(float*));
        if (payload->data[i] == NULL) {
            return false;
        }
    }
    payload->frames_allocated = frames_allocated;
    return true;
}

bool hantek_drc_mem_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_info* info = channel->info;
    hantek_drc_mem_payload* payload = (hantek_drc_mem_payload*) info->payload;
    if (payload->frames_allocated <= info->frame_count) {
        size_t new_frames_allocated = payload->frames_allocated * 2;
        for (size_t i = 0; i < info->channel_count; ++i) {
            payload->data[i] = realloc(payload->data[i], sizeof(float**)*new_frames_allocated);
            if (payload->data[i] != NULL) {
                memset(payload->data[i] + payload->frames_allocated, 0, 
                    sizeof(float**)*(new_frames_allocated - payload->frames_allocated));
            } else {
                return false;
            }
        }
        payload->frames_allocated = new_frames_allocated;
    }

    float* frame = malloc(sizeof(float) * info->buffer_length);
    if (frame == NULL) {
        return false;
    }
    for (size_t i = 0; i < info->buffer_length; ++i) {
        frame[i] = payload->data_fn(channel, buffer[i]);
    }
    payload->data[channel->index][info->frame_count] = frame;
    return true;
}

void hantek_drc_mem_free(hantek_drc_info* info) {
    hantek_drc_mem_payload* payload = (hantek_drc_mem_payload*) info->payload;
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
        info->payload = NULL;
    }
}

bool hantek_drc_mem_init(hantek_drc_info* info, hantek_drc_data_fn data_fn) {
    hantek_drc_mem_payload* payload = calloc(1, sizeof(hantek_drc_mem_payload));
    if (payload == NULL) {
        return false;
    }
    payload->data_fn = hantek_drc_data_fn_or_default(data_fn);
    info->payload = payload;
    info->on_prepare = &hantek_drc_mem_prepare;
    info->on_frame = &hantek_drc_mem_frame;
    info->on_free = &hantek_drc_mem_free;
    return true;
}
