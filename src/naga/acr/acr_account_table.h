#ifndef __ACR_ACCOUNT_TABLE_H__
#define __ACR_ACCOUNT_TABLE_H__

typedef struct {
    bts_ipaddr_t ip;
    char account[NAGA_ACCOUNT_STR_SZ];
} acr_account_entry_t;

berr acr_account_table_init(uint32_t number);
acr_account_entry_t* acr_account_table_lookup(bts_ipaddr_t ip);
uint32_t account_table_hash_func(void *data);


#endif /* !__NAGA_ACCOUNT_TABLE_H__ */
