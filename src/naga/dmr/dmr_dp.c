#include "dmr.h"
#include "dmr_dp.h"
#include "naga_types.h"
#include "bts_debug.h"

#include "boots.h"
#include "bts_cnt.h"

#define MAX_HOST_RULE_NUM 10000

extern uint32_t domin_default_action;

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
#if 0
	if (0 == (tag->acl.actions & ACT_PUSH))
	{
		return E_SUCCESS;
	}

	tag->acl.actions = tag->acl.actions & (~ACT_PUSH);
#endif	
	CNT_INC(DMR_PKTS);

    rule = dmr_get((char *)tag->host);

    if (NULL == rule)
    {
        CNT_INC(DMR_RULE_UNMATCH);
		HYTAG_ACL_SET(tag->acl, domin_default_action);
    }
    else
    {
        CNT_INC(DMR_RULE_MATCH);
        ACL_HIT(rule->acl);

		if (0 == (tag->acl.actions & ACT_DROP))
	
		{
			ACL_PRE_NOT_DROP_HIT(rule->acl);
		}
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
		printf("Domain rule init FAIL!\n");
	}
	else
	{
		printf("Domain rule init success!\n");
	}
	return;
}


/* End of file */ 
