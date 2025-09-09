#include "mem.h"

#include <stdlib.h>
#include <string.h>

bool hantek_drc_mem_prepare(hantek_drc_info* info) {
    hantek_drc_mem_params* params = (hantek_drc_mem_params*) info->frame_handler.params;
    params->data = calloc(info->channel_count, sizeof(void**));
    if (params->data == NULL) {
        return false;
    }

    size_t frames_allocated = 16;
    for (size_t i = 0; i < info->channel_count; ++i) {
        params->data[i] = calloc(frames_allocated, sizeof(void*));
        if (params->data[i] == NULL) {
            return false;
        }
    }
    params->frames_allocated = frames_allocated;
    return true;
}

bool hantek_drc_mem_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_info* info = channel->info;
    hantek_drc_mem_params* params = (hantek_drc_mem_params*) info->frame_handler.params;
    if (params->frames_allocated <= info->frame_count) {
        size_t frames_allocated = params->frames_allocated * 2;
        for (size_t i = 0; i < info->channel_count; ++i) {
            params->data[i] = realloc(params->data[i], sizeof(void**)*frames_allocated);
            if (params->data[i] != NULL) {
                memset(params->data[i] + params->frames_allocated, 0, 
                    sizeof(float**)*(frames_allocated - params->frames_allocated));
            } else {
                return false;
            }
        }
        params->frames_allocated = frames_allocated;
    }
    void* frame_data = hantek_drc_data_frame(channel, buffer);
    if (frame_data == NULL) {
        return false;
    }
    params->data[channel->index][info->frame_count] = frame_data;
    return true;
}

void hantek_drc_mem_free(hantek_drc_info* info) {
    hantek_drc_mem_params* params = (hantek_drc_mem_params*) info->frame_handler.params;
    if (params != NULL) {
        if (params->data != NULL) {
            for (size_t i = 0; i < info->channel_count; ++i) {
                if (params->data[i] != NULL) {
                    for (size_t j = 0; j < params->frames_allocated; ++j) {
                        if (params->data[i][j] != NULL) {
                            free(params->data[i][j]);
                        }
                    }
                    free(params->data[i]);
                }
            }
            free(params->data);
        }
        free(params);
        info->frame_handler.params = NULL;
    }
}

bool hantek_drc_mem_init(hantek_drc_info* info) {
    hantek_drc_mem_params* params = calloc(1, sizeof(hantek_drc_mem_params));
    if (params == NULL) {
        return false;
    }
    info->frame_handler = (hantek_drc_frame_handler) {
        .on_prepare = &hantek_drc_mem_prepare,
        .on_frame = &hantek_drc_mem_frame,
        .on_free = &hantek_drc_mem_free,
        .params = params
    };
    return true;
}
