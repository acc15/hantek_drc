#include <stdlib.h>
#include "handler.h"
#include "info.h"

void hantek_drc_handler_free(hantek_drc_info* info, hantek_drc_handler* handler) {
    if (handler->on_free != NULL) {
        handler->on_free(info);
        handler->on_free = NULL;
    }
    if (handler->params != NULL && handler->should_free) {
        free(handler->params);
        handler->params = NULL;
        handler->should_free = false;
    }
}
