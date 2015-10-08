#include "bts_hash.h"
#include "bts_hashtable.h"
#include "bts_linklist.h"
#include "bts_util.h"
#include "bts_debug.h"

#include "dmr.h"

bts_hashtable_t dmr_table;

uint32_t 
dmr_hash_func(void *data)
{
    dmr_t *entry = NULL;

    if (NULL == data)
    {
        return 0;
    }

    entry = (dmr_t *) data; 

    return bts_hash(entry->host, entry->host_len);
}

int 
dmr_cmp_func(void *d1, void *d2)
{
    dmr_t *e1, *e2;

    if ((NULL == d1) || (NULL == d2))
    {
        return -2;
    }

    e1 = (dmr_t *) d1;
    e2 = (dmr_t *) d2;

    return bts_str_cmp((void *)(e1->host), (void *)(e2->host));
}

berr
dmr_init(uint32_t number)
{
    return bts_hashtable_init(&dmr_table, number, dmr_hash_func, dmr_cmp_func, NULL);
}

dmr_t*
dmr_get(char *key)
{
    return (dmr_t *)bts_hashtable_lookup(&dmr_table, (void*) key);
}

berr
dmr_add(dmr_t *entry)
{
    return bts_hashtable_add(&dmr_table, (void *) entry);
}

berr
dmr_del(char *host)
{
    dmr_t *entry = NULL;

    entry = dmr_get(host);
    if (NULL == entry)
    {
        BRET(E_FOUND);
    }

    return bts_hashtable_del(&dmr_table, (void *) entry);
}

berr
dmr_clear(void)
{
    return bts_hashtable_del_all(&dmr_table);
}

void
dmr_stat_clear_func(void *data, void *param)
{
    dmr_t *dmr = NULL;
    if (NULL == data)
    {
        return;

    }
    dmr = (dmr_t *) data;

    bts_atomic64_set(&dmr->acl.cnt, 0); 
}

berr
dmr_stat_clear(char *host)
{
    dmr_t *dmr = NULL;

    dmr = dmr_get(host);

    if (NULL == dmr)
    {
        BRET(E_FOUND);
    }

    dmr_stat_clear_func(dmr, NULL);

    return E_SUCCESS;
}

berr
dmr_stat_clear_all(void)
{
    bts_hashtable_iter(&dmr_table, dmr_stat_clear_func, NULL);

    return E_SUCCESS;
}

berr
dmr_iter(bts_iter_func func, void *param)
{
    if (NULL == func)
    {
        BRET(E_PARAM);
    }

    bts_hashtable_iter(&dmr_table, func, param); 

    return E_SUCCESS;
}

/* End of file */
