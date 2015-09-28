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


naga_host_rule_t *dmr_api_rule_host_show(naga_host_rule_t *data)
{
	
    return naga_host_rule_lookup((void *)data);
}


berr dmr_api_rule_host_del_all(void)
{
	return naga_host_rule_clear();

}

