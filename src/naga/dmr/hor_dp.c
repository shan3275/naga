#include "naga_host_rule.h"

berr
naga_nor(hytag_t *tag)
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
