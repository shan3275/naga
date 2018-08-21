#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "url.h"

naga_rule_tab_t *ori_url_param_rules_tab = NULL;
naga_rule_tab_t *ref_url_param_rules_tab = NULL;

berr url_param_rule_add(naga_rule_tab_t *rules_tab, naga_rule_t *srule)
{
    naga_rule_t *drule = NULL;

    if (rules_tab->inuse >= rules_tab->size) {
        return E_EXCEED;
    }
    drule = rules_tab[rules_tab->inuse];

    memcpy(drule, srule, sizeof(naga_rule_t));
    memcpy(drule->data, srule->data, sizeof(naga_url_param_t)); 

    return E_SUCCESS;
}


naga_rule_t* url_param_rule_get(naga_rule_tab_t *rules_tab, uint32_t idx)
{
    uint32_t offidx = 0;

    if ((NULL == rules_tab) || (id < rules_tab->offset)) {
        return E_PARAM;
    }

    offidx  = idx - rules_tab->offset;

    if (offidx > rules_tab->inuse) 
    {
        return E_FOUND;
    }

    return &rules_tab[offidx];
}



berr url_param_rule_del(naga_rule_tab_t *rules_tab, uint32_t idx)
{
    uint32_t offidx = 0;

    if ((NULL == rules_tab) || (id < rules_tab->offset)) {
        return E_PARAM;
    }

    offidx  = idx - rules_tab->offset;

    if (offidx > rules_tab->inuse) 
    {
        return E_FOUND;
    }
    
    naga_rule_t *rule = rules_tab[offidx];

    memset(rule->data, 0, sizeof(naga_url_param_t)); 
    memset(rule, 0, sizeof(naga_rule_t));

    return E_SUCCESS;
}

berr naga_url_param_match(naga_url_param_t *rule_params, naga_url_param_t *pack_params)
{
    int i = 0, j = 0, hits = 0;
    char *rkey = NULL, *rval = NULL, *pkey = NULL, *pval = NULL;
    char *offset = NULL;

    if (NULL == pack_params || NULL == rule_params) {
        return E_FAIL;
    }

    if (pack_params->pnumb < rule_params) {
        return E_FAIL;
    }

    for (i = 0; i < rule_params; i++) {
        rkey = rule_params->pkeys[i];
        rval = rule_params->pvals[i];

        for (y = 0; y < pack_params; y++) {
            pkey = pack_params->pkeys[y];
            pval = pack_params->pvals[y];

            if (0 == strcmp(pkey, rkey)) {
                offset = strstr(pval, rval);
                if (offset > 0) {
                    hits += 1;
                }
            }
        }
    }

    if (hits == pack_params->pnumb) {
        return E_SUCCESS;
    }

    return E_FAIL;
}


berr  naga_url_param(hytag_t *hytag, naga_url_param_t *rules, uint32_t *rules_inuse)
{
    int hits = 0, y = 0;
    naga_url_param_t *rule_params = NULL, *pack_params = NULL;
    if (NULL == param) {
        return E_PARAM;
    }

    if (APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        return E_SUCCESS;
    }

    pack_params = hytag->ori_url.params;
   
    for(i=0; i< *rules_inuse; i++)
    {
        rule_params = &rules[i];

        if (E_SUCCESS == naga_url_param_match(rule_params, pack_params)) {
            hytag->rule_idxs[hytag->rule_hits];
            hytag->rule_hits += 1;
        }
    }

	return E_SUCCESS;  
}

berr naga_ori_url_param(hytag_t *hytag)
{
    return naga_url_param(ori_url_param_rules_tab, hytag);
}

berr naga_ref_url_param(hytag_t *hytag)
{
    return naga_url_param(ref_url_param_rules_tab, hytag);
}

berr ori_url_param_rule_get(uint32_t idx)
{
    return url_param_rule_get(ori_url_param_rules_tab, idx);
}

berr ref_url_param_rule_get(uint32_t idx)
{
    return url_param_rule_get(ref_url_param_rules_tab, idx);
}

berr ori_url_param_rule_add(naga_rule_t *rule)
{
    return url_param_rule_add(ori_url_param_rules_tab, rule);
}

berr ref_url_param_rule_add(naga_rule_t *rule)
{
    return url_param_rule_add(ref_url_param_rules_tab, rule);
}

berr ori_url_param_rule_del(uint32_t idx)
{
    return url_param_rule_del(ori_url_param_rules_tab, idx);
}

berr ref_url_param_rule_del(uint32_t idx)
{
    return url_param_rule_del(ref_url_param_rules_tab, idx);
}
/* End of file */

