#include <stdlib.h>
#include "handler.h"
#include "info.h"

void hantek_drc_handler_free(void* handler_any, const hantek_drc_info* info) {
    hantek_drc_handler* handler = (hantek_drc_handler*) handler_any;
    if (handler->on_free != NULL) {
        handler->on_free(handler->params, info);
        handler->on_free = NULL;
    }
    if (handler->params != NULL && handler->should_free) {
        free(handler->params);
        handler->params = NULL;
        handler->should_free = false;
    }
}
