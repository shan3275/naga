#include "naga_host_rule.h"

#include "bts_hash.h"
#include "bts_hashtable.h"
#include "bts_linklist.h"


bts_hashtable_t naga_host_rule;

uint32_t host_rule_hash_func(void *data)
{
    naga_host_rule_t *entry = NULL;

    if (NULL == data)
    {
        return 0;
    }

    entry = (naga_host_rule_t *) data; 

    return bts_hash(entry->host, entry->host_len);
}
#if 0
int 
account_entry_cmp_func(void *d1, void *d2)
{
    naga_host_rule_t *e1, *e2;

    if ((NULL == d1) || (NULL == d2))
    {
        return -2;
    }

    e1 = (naga_host_rule_t *) d1;
    e2 = (naga_host_rule_t *) d2;

    return bts_str_cmp(e1->account, d2->account);
}
#endif

berr 
naga_host_rule_init(uint32_t number)
{
    return bts_hashtable_init(&naga_host_rule, number, host_rule_hash_func, NULL, NULL);
}

naga_host_rule_t*
naga_host_rule_lookup(char* host)
{
    return (naga_host_rule_t *)bts_hashtable_lookup(&naga_host_rule, (void *) host);
}

berr
naga_host_rule_add(naga_host_rule_t *entry)
{
    return bts_hashtable_add(&naga_host_rule, (void *) entry);
}

berr
naga_host_rule_del(naga_host_rule_t *entry)
{
    return bts_hashtable_del(&naga_host_rule, (void *) entry);
}

#if 0
berr
naga_host_rule_clear(bts_hashrule_t *tab)
{
    return bts_hashrule_clear(tab);
}
#endif
/* End of file */
