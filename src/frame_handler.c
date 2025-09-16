#include "hantek_drc/frame_handler.h"


bool hantek_drc_frame_handler_prepare(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_info* info
) {
    return handler->on_prepare == NULL || handler->on_prepare(handler->params, info);
}

bool hantek_drc_frame_handler_frame(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_channel* channel, 
    const int16_t* buffer
) {
    return handler->on_frame == NULL || handler->on_frame(handler->params, channel, buffer);
}

bool hantek_drc_frame_handler_finish(
    hantek_drc_frame_handler* handler, 
    const hantek_drc_info* info
) {
    return handler->on_finish == NULL || handler->on_finish(handler->params, info);
}
