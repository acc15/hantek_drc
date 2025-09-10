#include "mem.h"

#include <stdlib.h>
#include <string.h>

hantek_drc_frame_handler hantek_drc_mem_handler(hantek_drc_mem_params* params) {
    return (hantek_drc_frame_handler) {
        .on_prepare = NULL,
        .on_frame = &hantek_drc_mem_frame,
        .on_free = &hantek_drc_mem_free,
        .params = params
    };
}
HANTEK_DRC_HANDLER_ALLOC_IMPL( mem, frame )

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
        mch->frames = realloc(mch->frames, sizeof(void**) * frames_allocated);
        if (mch->frames == NULL) {
            return false;
        }
        memset(mch->frames + mch->allocated, 0, sizeof(void**) * (frames_allocated - mch->allocated));
        mch->allocated = frames_allocated;
    }
    
    void* frame_data = hantek_drc_format_frame(&params->format, channel, buffer);
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
        params->channels[i].frames = NULL;
    }
}


