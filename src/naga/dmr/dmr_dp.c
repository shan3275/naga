#include "dmr.h"
#include "dmr_dp.h"
#include "naga_types.h"
#include "bts_debug.h"

#include "boots.h"
#include "bts_cnt.h"

#define MAX_HOST_RULE_NUM 10000

berr naga_dmr(hytag_t *tag)
{
    dmr_t* rule = NULL;

    if (NULL == tag)
    {
        BRET(E_FAIL);
    }

	if( APP_TYPE_HTTP_GET_OR_POST != tag->app_type)
    {
        return E_SUCCESS;
    }

	CNT_INC(DMR_PKTS);

    rule = dmr_get((char *)tag->host);

    if (NULL == rule)
    {
        CNT_INC(DMR_RULE_UNMATCH);
    }
    else
    {
        CNT_INC(DMR_RULE_MATCH);
        ACL_HIT(rule->acl);
        HYTAG_ACL_MERGE(tag->acl, rule->acl);
    }

    BRET(E_SUCCESS);
}

void dmr_dp_init(void)
{
	berr rv;
	rv = dmr_init(MAX_HOST_RULE_NUM);
	if (E_SUCCESS != rv)
	{
		printf("Host rule init FAIL!\n");
		return;
	}
}


/* End of file */ 
