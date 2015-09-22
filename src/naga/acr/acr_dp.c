#include "naga.h"
#include "naga_account_table.h"
#include "naga_account_rule.h"

void
acr_account_fill(hytag_t *tag)
{
    naga_account_entry_t *entry = NULL;

    entry = naga_account_table_lookup(tag->inner_srcip);

    if (NULL == entry)
    {
        CNT_INC(ACR_TABLE_NOTFOUND);
        bts_ip_string(tag->account, tag->inner_srcip);
    }
    else
    {
        CNT_INC(ACR_TABLE_FOUND);
        strcpy(tag->account, entry->account);
    }
}

berr
naga_acr_init()
{
    acr_account_table_init();
}


/* End of file */
berr
naga_acr(hytag_t *tag)
{
    naga_account_rule_t* rule = NULL;
    assert(tag);

    CNT_INC(ACR_PKTS);
    acr_account_fill(tag);

    rule = naga_account_rule_lookup(tag->account);

    if (NULL == rule)
    {
        CNT_INC(ACR_RULE_UNMATCH);
    }
    else
    {
        CNT_INC(ACR_RULE_MATCH);
        ACL_HIT(rule->acl);
        HYTAG_ACL_MERGE(tag->acl, rule->acl);
    }

    BRET(E_SUCCESS);
}

/* End of file */
