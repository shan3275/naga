#include "boots.h"

char* berr_msg(berr e)
{
    if (e >= E_MAX)
    {
        return berr_array[E_MAX];
    }

    return berr_array[e].abbr;
}

char* bode_abbr(bmod m)
{
    if (m >= MOD_MAX)
    {
        return mod_array[MOD_MAX];
    }

    return mod_array[e];
}


/* End of file */
