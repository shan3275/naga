/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_api.c
#       @date         :2015-08-22 21:35
#       @algorithm    :
=============================================================================*/
#include "dmr_api.h"
#include "bts_cnt.h"
#include "bts_hash.h"
#include "naga_host_rule.h"




berr  dmr_api_rule_host_add(naga_host_rule_t *entry)
{
    return naga_host_rule_add(entry);
}


berr  dmr_api_rule_host_del(naga_host_rule_t *entry)
{	
    return naga_host_rule_del(entry);
}


naga_host_rule_t *dmr_api_rule_host_show(naga_host_attr_t *host)
{
	
    return naga_host_rule_lookup(&host);
}





/* dp use */
berr dmr_dp_api_match(uint8_t *host, uint16_t len)
{
 	return E_SUCCESS;
}

berr dmr_dp_api_init()
{
    
    return E_SUCCESS;

}

berr dmr_dp_api_request_data_entry(void)
{
    return E_SUCCESS;
}
