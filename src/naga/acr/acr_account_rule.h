#ifndef __NAGA_ACCOUNT_RULE_H__
#define __NAGA_ACCOUNT_RULE_H__

typedef struct {
    uint32_t idx;
    char account[NAGA_ACCOUNT_STR_SZ];
    naga_acl_t acl;
} acr_account_entry_t;

berr acr_account_table_init(uint32_t number);
acr_account_entry_t* acr_account_table_lookup(bts_ipaddr_t ip);

#endif /* !__ACR_ACCOUNT_RULE_H__ */
