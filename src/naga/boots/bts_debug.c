#include "boots.h"
#include "bts_debug.h"

char *berr_array[E_MAX + 1] = {
    "SUCCESS",
    "FAIL",
    "NULL",
    "MATCH",    // ???
    "EFFECTIVE",
    "MEMORY",   // ???? 
    "PARAM",    // ????
    "EXCEED",   // ?? 
    "OPEN",     // ????
    "FOUND",    // ???
    "EXIST",    // ????
    "BUSY",     // ?? 
    "COMPARE",
    "INIT",
    "FORMAT",
    "UNKOWN",
};

char* berr_msg(berr e)
{
    if (e >= E_MAX)
    {
        return berr_array[E_MAX];
    }

    return berr_array[e];
}

mod_t mod_array[MOD_MAX] = {
    {MOD_DBG, BAS, OFF, OFF, "dbg", "debug module"},
#ifdef MOD_ARRAY_CUSTOM
    MOD_ARRAY_CUSTOM,
#endif
} ;

dopt_t dopts_array[DOPT_MAX] = {
      {DOPT_INFO,     OFF, "info",      "normal debug message"},
      {DOPT_VERB,     ON,  "verb",      "verbose debug message"},
      {DOPT_WARN,     ON,  "warn",      "warning message"},
      {DOPT_ERR,      ON,  "err",       "error message"},
      {DOPT_ETRACE,   ON,  "etrace",    "error return trace"},
      {DOPT_TAGMON,   OFF,  "tagmon",    "hytag monitor"},
      {DOPT_TINYSTEP, ON,  "tinystep",  "time spand of a section code"},
      {DOPT_TRAP,     ON,  "trap",      "a trap trigger by condition"},
#ifdef DOPT_ARRAY_CUSTOM
      DOPT_ARRAY_CUSTOM,
#endif
} ;

mod_t*
mod_lookup(char *name)
{
    int i;
    if (NULL == name)
    {
        return NULL;
    }

    for (i = 0; i < MOD_MAX; i++)
    {
        if (!strcmp(name, mod_array[i].abbr))
        {
            return &mod_array[i];
        }
    }

    return 0;
}

int mod_is_enable(bmod mod)
{
   return (mod < MOD_MAX) ?  mod_array[mod].enable : OFF;
}


int dopts_is_enable(dopte dop)
{
    return dop < DOPT_MAX ? dopts_array[dop].enable : OFF;
}

dopt_t*
dopt_lookup(char *name)
{
    int i;
    if (NULL == name)
    {
        return NULL;
    }

    for (i = 0; i < MOD_MAX; i++)
    {
        if (!strcmp(name, dopts_array[i].name))
        {
            return &dopts_array[i];
        }
    }

    return 0;
}

/* End of file */
