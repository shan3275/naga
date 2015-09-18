
#include <string.h>
#include "bts_hashtable.h"



berr 
bts_hashtable_init(bts_hashtable_t *tab, uint32_t bucket_number, bts_hash_func hash, bts_list_cmp_func cmp, bts_list_del_func del)
{
	int i;
    if ((NULL == tab) || (NULL == hash))
    {
        BRET(E_PARAM);
    }

    tab->hash = hash;
   

    memset(tab, 0, sizeof(bts_hashtable_t));

    //tab->buckets = malloc(tab->buckets, 0 , sizeof(bts_hashtable_t) * bucket_number);
    tab->buckets = malloc(sizeof(bts_list_t) * bucket_number);

    if (NULL == tab->buckets)
    {
        BRET(E_MEMORY);
    }

    memset(tab->buckets, 0, sizeof(bts_list_t) * bucket_number);

    if (cmp || del)
    {
        for (i = 0; i < bucket_number; i++)
        {
            tab->buckets[i].cmp = cmp;
            tab->buckets[i].del = del;
        }
    }

    BRET(E_SUCCESS);
}

void *
bts_hashtable_lookup(bts_hashtable_t *tab, void *key)
{
    uint32_t hash, idx;
    bts_list_t *bucket = NULL;
    bts_listnode_t *node = NULL;

    if ((NULL == tab) || (NULL == key))
    {
        return NULL;
    }

    if ((NULL == tab->hash) || (0 == tab->total_bucket) || (NULL == tab->buckets))
    {
        return NULL;
    }

    hash = tab->hash(key);

    idx = hash % tab->total_bucket;

    bucket = &tab->buckets[idx];

    node = bts_listnode_lookup_by_key(bucket, key);

    if (node)
    {
        return node->data;
    }

    return NULL;
}

berr
bts_hashtable_add(bts_hashtable_t *tab, void *data)
{
    uint32_t hash, idx;
    bts_list_t *bucket = NULL;
    bts_listnode_t *node = NULL;

    if ((NULL == tab) || (NULL == data))
    {
        BRET(E_PARAM);
    }

    if ((NULL == tab->hash) || (0 == tab->total_bucket) || (NULL == tab->buckets))
    {
        BRET(E_INIT);
    }

    hash = tab->hash(data);

    idx = hash % tab->total_bucket;

    bucket = &tab->buckets[idx];

    bts_listnode_add(bucket, data);

    BRET(E_SUCCESS);
}


berr
bts_hashtable_del(bts_hashtable_t *tab, void *data)
{
    uint32_t key, idx;
    bts_list_t *bucket = NULL;
    bts_listnode_t *node = NULL;

    if ((NULL == tab) || (NULL == data))
    {
        BRET(E_PARAM);
    }

    if ((NULL == tab->hash) || (0 == tab->total_bucket) || (NULL == tab->buckets))
    {
        BRET(E_INIT);
    }
    
    key = tab->hash(data);

    idx = key % tab->total_bucket;

    bucket = &tab->buckets[idx];

    bts_listnode_delete(bucket, data);

    BRET(E_SUCCESS);
}

berr
bts_hashtable_del_by_key(bts_hashtable_t *tab, void *data)
{
    uint32_t key, idx;
    bts_list_t *bucket = NULL;
    bts_listnode_t *node = NULL;

    if ((NULL == tab) || (NULL == data))
    {
        BRET(E_PARAM);
    }

    if ((NULL == tab->hash) || (0 == tab->total_bucket) || (NULL == tab->buckets))
    {
        BRET(E_INIT);
    }
    
    key = tab->hash(data);

    idx = key % tab->total_bucket;

    bucket = &tab->buckets[idx];

    node = bts_listnode_lookup_by_key(bucket, &key);

    if (node)
    {
        bts_list_delete_node(bucket, node);
    }

    BRET(E_SUCCESS);
}

/* End of file */
