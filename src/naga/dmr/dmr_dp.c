#include "naga_host_rule.h"
#include "dmr_dp.h"
#include "naga_types.h"
#include "bts_debug.h"

#include "boots.h"
#include "bts_cnt.h"


#define MAX_HOST_RULE_NUM 10000


static void dmr_host_fill(hytag_t *tag, naga_host_attr_t *host_attr)
{
	memcpy(host_attr->host, tag->host, MAX_HOST_LEN);
	host_attr->host_len = tag->host_len;
}

berr dmr_dp_match(hytag_t *tag)
{
    naga_host_rule_t* rule = NULL;
	naga_host_attr_t host_attr;
    if (NULL == tag)
    {
        return E_FAIL;
    }

	memset(&host_attr, 0, sizeof(naga_host_attr_t));
    //CNT_INC(ACR_PKTS);
    dmr_host_fill(tag, &host_attr);

    rule = naga_host_rule_lookup(&host_attr);

    if (NULL == rule)
    {
        //CNT_INC(ACR_RULE_UNMATCH);
    }
    else
    {
        //CNT_INC(ACR_RULE_MATCH);
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
