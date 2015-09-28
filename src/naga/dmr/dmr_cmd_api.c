/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_cmd_api.c
#       @date         :2015-08-22 19:38
#       @algorithm    :
=============================================================================*/

//#include "vsr_cmd_api.h"
#include "dmr_api.h"
#include "naga_host_rule.h"



berr rule_dmr_cmd_add_host(naga_host_rule_t *entry)
{
	return dmr_api_rule_host_add(entry);
}

berr rule_dmr_cmd_del_host(naga_host_rule_t *entry)
{
	return dmr_api_rule_host_del(entry);
}


naga_host_rule_t *rule_dmr_cmd_show_host(naga_host_rule_t *data)
{
	return dmr_api_rule_host_show(data);
}



berr rule_dmr_cmd_del_all(void)
{
    return dmr_api_rule_host_del_all();
}



