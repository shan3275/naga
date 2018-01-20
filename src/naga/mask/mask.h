#ifndef __MASK_H_
#define __MASK_H_

#include "naga_types.h"

#define MAX_MASK_RULE   64

typedef struct {
    uint32_t id;
    uint32_t used;
    uint32_t mask;
    naga_acl_t acl;
} mask_rule_t;

typedef struct
{
    uint32_t inuse;
    mask_rule_t rules[MAX_MASK_RULE] ;  
} mask_rule_tab_t;

berr  naga_mask(hytag_t *hytag);

berr mask_rule_add(uint32_t id, uint32_t mask, naga_acl_t *acl);

berr mask_rule_del(uint32_t id);

mask_rule_t* mask_rule_get(uint32_t id);

#endif
