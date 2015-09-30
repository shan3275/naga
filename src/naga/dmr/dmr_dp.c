#include "naga_host_rule.h"
#include "dmr_dp.h"
#include "naga_types.h"
#include "bts_debug.h"

#include "boots.h"
#include "bts_cnt.h"


#define MAX_HOST_RULE_NUM 10000


static void dmr_host_fill(hytag_t *tag, naga_host_rule_t *host)
{
	memcpy(host->host, tag->host, tag->host_len);
	host->host_len = tag->host_len;
}

berr naga_dmr(hytag_t *tag)
{
    naga_host_rule_t* rule = NULL;
	naga_host_rule_t host_info;
    if (NULL == tag)
    {
        return E_FAIL;
    }

	CNT_INC(DMR_PKTS);
	memset(&host_info, 0, sizeof(naga_host_rule_t));
    dmr_host_fill(tag, &host_info);

    rule = naga_host_rule_lookup((void *)(&host_info));

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
	rv = naga_host_rule_init(MAX_HOST_RULE_NUM);
	if (E_SUCCESS != rv)
	{
		printf("Host rule init FAIL!\n");
		return;
	}

}


/* End of file */ 
