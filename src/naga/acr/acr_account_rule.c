
#include "bts_hashrule.h"

bts_hashrule_t acr_account_rule;

uint32_t
account_rule_hash_func(void *data)
{
    acr_account_rule_t *entry = NULL;

    if (NULL == data)
    {
        return 0;
    }

    entry = (acr_account_rule_t *) data; 

    return bts_hash(entry->account, strlen(entry->account));
}

int
account_entry_cmp_func(void *d1, void *d2)
{
    acr_account_rule_t *e1, *e2;

    if ((NULL == d1) || (NULL == d2))
    {
        return -2;
    }

    e1 = (acr_account_rule_t *) d1;
    e2 = (acr_account_rule_t *) d2;

    return bts_str_cmp(e1->account, d2->account);
}

berr 
acr_account_rule_init(uint32_t number)
{
    return bts_hashrule_init(tab, number, account_rule_hash_func, account_entry_cmp_func, NULL);
}

acr_account_rule_t*
acr_account_rule_lookup(char* account)
{
    return (acr_account_rule_t*) bts_hashrule_lookup(&acr_account_rule, (void *) account);
}

berr
acr_account_rule_add(acr_account_rule_t *entry)
{
    return bts_hashrule_add(&acr_account_rule, (void *) entry);
}

berr
acr_account_rule_del(acr_account_rule_t *entry)
{
    return bts_hashrule_del(&acr_account_rule, (void *) entry);
}

berr
acr_account_rule_clear(bts_hashrule_t *tab)
{
    return bts_hashrule_clear(tab);
}

/* End of file */
