#ifndef __BTS_HASHTABLE_H__
#define __BTS_HASHTABLE_H__

#include "bts_linklist.h"
#include "boots.h"

typedef  uint32_t (*bts_hash_func)(void *data);

typedef struct {
    bts_hash_func hash;
    uint32_t total_bucket;
    uint32_t total_cell;
    bts_list_t *buckets;
} bts_hashtable_t;

berr bts_hashtable_init(bts_hashtable_t *tab, uint32_t bucket_number, bts_hash_func hash, bts_list_cmp_func cmp, bts_list_del_func del);

void *bts_hashtable_lookup(bts_hashtable_t *tab, void *key);
berr bts_hashtable_add(bts_hashtable_t *tab, void *data);
berr bts_hashtable_del(bts_hashtable_t *tab, void *data);


#endif /* !__BTS_HASHTABLE_H__ */
