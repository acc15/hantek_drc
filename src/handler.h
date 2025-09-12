#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hantek_drc_handler {
    void(*on_free)(void* params_any, hantek_drc_info* info);
    void* params;
    bool should_free;
} hantek_drc_handler;

void hantek_drc_handler_free(void* handler_any, hantek_drc_info* info);

#define HANTEK_DRC_HANDLER_ALLOC_DEF( name, type ) \
hantek_drc_ ## type ## _handler hantek_drc_ ## name ## _handler_alloc(hantek_drc_ ## name ## _params params_init)

#define HANTEK_DRC_HANDLER_ALLOC_IMPL( name, type ) \
HANTEK_DRC_HANDLER_ALLOC_DEF( name, type ) { \
    hantek_drc_ ## name ## _params* params = (hantek_drc_ ## name ## _params*) \
        calloc(1, sizeof(hantek_drc_ ## name ## _params)); \
    if (params == NULL) { \
        return (hantek_drc_ ## type ## _handler) {0}; \
    } \
    *params = params_init; \
    hantek_drc_ ## type ## _handler handler = hantek_drc_ ## name ## _handler(params); \
    handler.should_free = true; \
    return handler; \
}

#ifdef __cplusplus
}
#endif
