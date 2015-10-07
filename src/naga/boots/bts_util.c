#include "stdio.h"
#include "boots.h"
#include "bts_debug.h"
#include "naga_types.h"

int
bts_ipaddr_cmp(void *val1, void *val2)
{
    bts_ipaddr_t *ip1, *ip2;

    ip1 = (bts_ipaddr_t *) val1;
    ip2 = (bts_ipaddr_t *) val2;

    if ((NULL== ip1) || (NULL == ip2))
    {
        return -2;
    }

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

int
bts_str_cmp(void *val1, void *val2)
{
    if ((NULL== val1) || (NULL == val2))
    {
        return -2;
    }

    return strcmp(val1, val2);
}


int action_str2int(const char *str, uint32_t *action)
{
	if (!strcmp("push", str))
	{
		*action |= ACT_HTTP_RESPONSE;
	}
	else if (!strcmp("drop", str))
	{
		*action |= ACT_DROP;
	}
	else
	{
		BRET(E_FAIL);
	}

	return E_SUCCESS;
}


void bts_ip_string(uint32_t ip, char *str)
{
    sprintf(str, "%d.%d.%d.%d",
            ((ip >> 24) & 0xff),
            ((ip >> 16) & 0xff),
            ((ip >> 8) & 0xff),
            (ip & 0xff));

}





/* End of file */
