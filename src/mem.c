#include "mem.h"

#include <stdlib.h>
#include <string.h>

bool hantek_drc_mem_frame(hantek_drc_channel* channel, const int16_t* buffer) {
    hantek_drc_info* info = channel->info;
    hantek_drc_mem_params* params = (hantek_drc_mem_params*) info->frame_handler.params;
    hantek_drc_mem_channel* mch = &params->channels[channel->index];
    
    static const size_t INITIAL_FRAME_COUNT = 16;

    if (mch->allocated <= mch->recorded) {
        size_t frames_allocated = mch->allocated * 2;
        if (frames_allocated == 0) {
            frames_allocated = INITIAL_FRAME_COUNT;
        }
        mch->frames = realloc(mch->frames, sizeof(void**)*frames_allocated);
        if (mch->frames == NULL) {
            return false;
        }
        memset(mch->frames + mch->allocated, 0, sizeof(void**) * (frames_allocated - mch->allocated));
        mch->allocated = frames_allocated;
    }
    
    void* frame_data = hantek_drc_data_frame(channel, buffer);
    if (frame_data == NULL) {
        return false;
    }

    mch->frames[mch->recorded] = frame_data;
    ++mch->recorded;
    return true;
}

void hantek_drc_mem_free(hantek_drc_info* info) {
    hantek_drc_mem_params* params = (hantek_drc_mem_params*) info->frame_handler.params;
    if (params == NULL) {
        return;
    }
    for (size_t i = 0; i < info->channel_count; ++i) {
        if (params->channels[i].frames == NULL) {
            continue;
        }
        for (size_t j = 0; j < params->channels[i].allocated; ++j) {
            if (params->channels[i].frames[j] != NULL) {
                free(params->channels[i].frames[j]);
            }
        }
        free(params->channels[i].frames);
    }
}

bool hantek_drc_mem_alloc(hantek_drc_info* info) {
    hantek_drc_mem_params* params = calloc(1, sizeof(hantek_drc_mem_params));
    if (params == NULL) {
        return false;
    }
    hantek_drc_mem_ext(info, params);
    info->frame_handler.should_free = true;
    return true;
}

bool hantek_drc_mem_ext(hantek_drc_info* info, hantek_drc_mem_params* params) {
    info->frame_handler.on_prepare = NULL;
    info->frame_handler.on_frame = &hantek_drc_mem_frame;
    info->frame_handler.on_free = &hantek_drc_mem_free;
    info->frame_handler.params = params;
    return true;
}
