#ifndef __HOR_DOMAIN_RULE_H__
#define __HOR_DOMAIN_RULE_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "naga_types.h"
#include "boots.h"

typedef struct {
    uint32_t index;
    uint32_t host_len;
    char host[32];
    naga_acl_t acl;
} naga_host_rule_t;

uint32_t host_rule_hash_func(void *data);
berr naga_host_rule_init(uint32_t number);
naga_host_rule_t *naga_host_rule_lookup(char* host);
berr naga_host_rule_add(naga_host_rule_t *entry);
berr naga_host_rule_del(naga_host_rule_t *entry);




#endif /* !__HOR_DOMAIN_RULE_H__ */
