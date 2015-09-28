//#include "naga_account_table.h"
#include "string.h"
#include "acr_account_rule.h"
#include "acr_account_table.h"
#include "naga_types.h"
#include "bts_debug.h"

#include "boots.h"
#include "bts_cnt.h"


#define MAX_ACCOUNT_RULE_NUM 10000



void
acr_account_fill(hytag_t *tag, acr_account_rule_t *data)
{
    acr_account_entry_t *entry = NULL;

    entry = acr_account_table_lookup(tag->inner_srcip4);

    if (NULL == entry)
    {
        //CNT_INC(ACR_TABLE_NOTFOUND);
        bts_ip_string(tag->inner_srcip4, tag->account);
    }
    else
    {
        //CNT_INC(ACR_TABLE_FOUND);
        strcpy(tag->account, entry->account);
    }

	strcpy(data->account, tag->account);
	data->account_len = strlen(tag->account);
	
}


void acr_dp_init(void)
{
	berr rv;
    rv = acr_account_rule_init(MAX_ACCOUNT_RULE_NUM);
	if (E_SUCCESS != rv)
	{
		printf("Host rule init FAIL!\n");
		return;
	}
}


/* End of file */
berr
naga_acr(hytag_t *tag)
{
    acr_account_rule_t* rule = NULL;
	acr_account_rule_t data;
    if (NULL == tag)
    {
		return E_FAIL;
	}
    //CNT_INC(ACR_PKTS);
    memset(&data, 0, sizeof(acr_account_rule_t));
    acr_account_fill(tag, &data);

    rule = acr_account_rule_lookup(tag->account);

    if (NULL == rule)
    {
        //CNT_INC(ACR_RULE_UNMATCH);
        ;
    }
    else
    {
        //CNT_INC(ACR_RULE_MATCH);
        ACL_HIT(rule->acl);
        HYTAG_ACL_MERGE(tag->acl, rule->acl);
    }

    BRET(E_SUCCESS);
}

/* End of file */
