#ifndef __NAGA_HOST_RULE_H__
#define __NAGA_HOST_RULE_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "naga_types.h"
#include "boots.h"


typedef struct {

} naga_host_attr_t;

typedef struct {
    uint32_t index;
	uint32_t host_len;
    uint8_t host[MAX_HOST_LEN];
    naga_acl_t acl;
} naga_host_rule_t;

uint32_t host_rule_hash_func(void *data);
int host_entry_cmp_func(void *d1, void *d2);
berr naga_host_rule_init(uint32_t number);
naga_host_rule_t *naga_host_rule_lookup(void *host);
berr naga_host_rule_add(naga_host_rule_t *entry);
berr naga_host_rule_del(naga_host_rule_t *entry);
berr naga_host_rule_clear(void);





#endif /* !__HOR_DOMAIN_RULE_H__ */
