/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/acr\acr_api.c
#       @date         :2015-08-22 21:35
#       @algorithm    :
=============================================================================*/

#include "acr_api.h"
#include "bts_cnt.h"
#include "bts_hash.h"
#include "acr_account_rule.h"



berr  acr_api_rule_account_add(acr_account_rule_t *entry)
{
    return acr_account_rule_add(entry);
}


berr  acr_api_rule_account_del(acr_account_rule_t *entry)
{	
    return acr_account_rule_del(entry);
}


acr_account_rule_t *acr_api_rule_account_show(acr_account_rule_t *data)
{
	
    return acr_account_rule_lookup((void *)data);
}


berr acr_api_rule_account_del_all(void)
{
	return acr_account_rule_clear();

}



/* dp use */
berr acr_dp_api_match(uint8_t *account, uint16_t len)
{
 	return E_SUCCESS;
}

berr acr_dp_api_init()
{
    
    return E_SUCCESS;

}

berr acr_dp_api_request_data_entry(void)
{
    return E_SUCCESS;
}

