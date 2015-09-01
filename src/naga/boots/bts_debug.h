#ifndef __BTS_DEBUG_H__
#define __BTS_DEBUG_H__

#include "boots.h"
#include "stdio.h"

typedef enum {
      DOPT_INFO,
      DOPT_VERB,
      DOPT_WARN,
      DOPT_ERR,
      DOPT_ETRACE,
      DOPT_TAGMON,
      DOPT_TINYSTEP,
      DOPT_TRAP,
      DOPT_CALLSTACK,
#ifdef DOPT_CUSTOM
      DOPT_CUSTOM,
#endif
      DOPT_MAX
} dopte;

typedef struct {
      dopte idx;
      uint32_t enable;
      char *name;
      char *desc;
} dopt_t;

dopt_t dopts_array[DOPT_MAX] = {
      {DOPT_INFO,     OFF, "info",      "normal debug message"},
      {DOPT_VERB,     OFF, "verb",      "verbose debug message"},
      {DOPT_WARN,     OFF, "warn",      "warning message"},
      {DOPT_ERR,      OFF, "err",       "error message"},
      {DOPT_ETRACE,   OFF, "etrace",    "error return trace"},
      {DOPT_TAGMON,   OFF, "tagmon",    "hytag monitor"},
      {DOPT_TINYSTEP, OFF, "tinystep",  "time spand of a section code"},
      {DOPT_TRAP,     OFF, "trap",      "a trap trigger by condition"},
#ifdef DOPT_ARRAY_CUSTOM
      DOPT_ARRAY_CUSTOM,
#endif
};

#define BTS_DEBUG_DUMP(_mod, _opt, _fmt, _args...) { \
    if ((_mod < DOPT_MAX) || (_opt < DOPT_MAX)) { \
        if (dopts_array[_opt].enable && dopts_array[_mod].enable) { \
            printf("[%s.%s] %s.%d:" _fmt, #_mod, #_opt, __FUNCTION__, __LINE__, ##_args); \
        } \
    } \
}

#define DBG_INFO(_mod, _fmt, _args...)   \
    BTS_MOD_DEBUG_DUMP(_mod, DOPT_INFO, _fmt, ##_args) 

#define DBG_VERB(_mod, _fmt, _args...)  \
    BTS_MOD_DEBUG_DUMP(_mod, DOPT_VERB, _fmt, ##_args) 

#define DBG_WARN(_mod, _fmt, _args...) \
    BTS_MOD_DEBUG_DUMP(_mod, DOPT_WARN, _fmt, ##_args) 

#define DBG_ERR(_mod, _fmt, _args...) \
    BTS_MOD_DEBUG_DUMP(_mod, DOPT_ERR, _fmt, ##_args) 

#define BRET(e) { \
    berr _rv = (e);\
    if ((_rv != E_SUCCESS) && dopts_array[DOPT_ETRACE].enable) \
    { \
        printf("[ETRACE] %s.%d: return %d!", __FUNCTION__, __LINE__, berr_msg(_rv)); \
    } \
}

#endif
