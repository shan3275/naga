#include "boots.h"

int
bts_ipaddr_cmp(void *val1, void *val2)
{
    bts_ipaddr_t *ip1, *ip2;
    if ((NULL== ip1) || (NULL == ip2))
    {
        return -2;
    }

    ip1 = (bts_ipaddr_t *) val1;
    ip2 = (bts_ipaddr_t *) val2;

    if (*ip1 > *ip2)
    {
        return 1;
    }

    if (*ip1 < *ip2)
    {
        return -1;
    }

    return 0;
}

/* End of file */
