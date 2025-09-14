#pragma once

#include "handler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_frame_handler {
    struct hantek_drc_handler;
    bool(*on_frame)(void* params_any, const hantek_drc_channel* channel, const int16_t* buffer);
    bool(*on_prepare)(void* params_any, const hantek_drc_info* info);
    bool(*on_frame_prepare)(void* params_any, const hantek_drc_info* info);
    bool(*on_frame_finish)(void* params_any, const hantek_drc_info* info);
} hantek_drc_frame_handler;

bool hantek_drc_frame_handler_prepare(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_info* info
);

bool hantek_drc_frame_handler_frame(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_channel* channel, 
    const int16_t* buffer
);

bool hantek_drc_frame_handler_frame_prepare(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_info* info
);

bool hantek_drc_frame_handler_frame_finish(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_info* info
);

#ifdef __cplusplus
}
#endif
