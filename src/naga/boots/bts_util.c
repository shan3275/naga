#include "boots.h"
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

    return 0;
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
		return E_FAIL;
	}

	return E_SUCCESS;
}



/* End of file */
