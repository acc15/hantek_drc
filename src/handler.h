#pragma once

#include <stdbool.h>

#include "fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_handler {
    void(*on_free)(hantek_drc_info* info);
    void* params;
    bool should_free;
} hantek_drc_handler;

void hantek_drc_handler_free(hantek_drc_info* info, hantek_drc_handler* handler);

#ifdef __cplusplus
}
#endif
