#include "boots.h"

char *berr_array[E_MAX] = {
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

#if 1

mod_t mod_array[MOD_MAX] = {
    {MOD_DBG, BAS, OFF, OFF, "dbg", "debug module"},
#ifdef MOD_ARRAY_CUSTOM
    MOD_ARRAY_CUSTOM,
#endif
} ;

int get_mod_switch(bmod mod)
{
   return (mod < MOD_MAX) ?  mod_array[mod].swt : OFF;
}

#endif

#if 0
char* bode_abbr(bmod m)
{
    if (m >= MOD_MAX)
    {
        return mod_array[MOD_MAX];
    }

    return mod_array[e];
}
#endif


/* End of file */
