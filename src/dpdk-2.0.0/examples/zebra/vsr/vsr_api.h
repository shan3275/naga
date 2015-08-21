/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_api.h
#       @date         :2015/08/21 22:53
#       @algorithm    :
=============================================================================*/
#ifndef __VSR_API_H__
#define __VSR_API_H__

uint32_t vsr_api_get_ip_by_index(uint32_t index);
uint32_t vsr_api_get_mobile_by_index(uint32_t index);
berr vsr_api_add_rule(uint32_t index, uint32_t ip, uint32_t msisdn);
berr vsr_api_del_rule_by_index(uint32_t index);
berr vsr_api_get_rule_entry(uint32_t index, vsr_rule_entry *rule);
berr vsr_api_clear_statistics(uint32_t index);


#endif
