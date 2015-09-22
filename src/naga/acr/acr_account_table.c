#include "bts_hashtable.h"

bts_hashtable_t acr_account_table;

uint32_t 
account_table_hash_func(void *data)
{
    acr_account_entry_t *entry = NULL;

    if (NULL == data)
    {
        return 0;
    }

    entry = (acr_account_entry_t *)data; 

    return bts_hash(entry->ip, strlen(entry->ip));
}

int 
account_entry_cmp_func(void *d1, void *d2)
{
    acr_account_entry_t *e1, *e2;

    if ((NULL == d1) || (NULL == d2))
    {
        return -2;
    }

    e1 = (acr_account_entry_t *) d1;
    e2 = (acr_account_entry_t *) d2;

    return bts_ip_cmp(e1->ip, d2->ip);
}

berr 
acr_account_table_init(uint32_t number)
{
    return bts_hashtable_init(tab, number, account_table_hash_func, account_entry_cmp_func, NULL);
}

acr_account_entry_t*
acr_account_table_lookup(bts_ipaddr_t ip)
{
    return (acr_account_entry_t*) bts_hashtable_lookup(&acr_account_table, (void *) ip);
}

berr
acr_account_table_add(acr_account_entry_t *entry)
{
    return bts_hashtable_add(&acr_account_table, (void *) entry);
}

berr
acr_account_table_del(acr_account_entry_t *entry)
{
    return bts_hashtable_del(&acr_account_table, (void *) entry);
}

berr
acr_account_table_clear(bts_hashtable_t *tab)
{
    return bts_hashtable_clear(tab);
}

/* End of file */
