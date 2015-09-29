#ifndef __DBG_TAGMON_H__
#define __DBG_TAGMON_H__

#include "pid.h"
#include "boots_custom.h"
#include "bts_debug.h"


void hytag_print(hytag_t *tag);
void hytag_log(hytag_t *tag);

#define DOPT_IS_SELECTED(dop)    (dopts_get_switch(dop) == ENABLE)

#define HYTAG_DUMP(_tag) \
{ \
    if (DOPT_IS_SELECTED(DOPT_TAGMON)) \
    { \
        hytag_print(_tag); \
    } \
}

#define HYTAG_LOG(_tag) \
{ \
    if (DOPT_IS_SELECTED(DOPT_TAGMON)) \
    { \
        hytag_log(_tag); \
    } \
}


#endif
