/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/acr\acr_api.h
#       @date         :2015/08/21 22:53
#       @algorithm    :
=============================================================================*/
//#include "bts_cnt.h"



#ifndef __ACR_API_H__
#define __ACR_API_H__

#include "acr_account_rule.h"
#include "boots.h"




berr  acr_api_rule_account_add(acr_account_rule_t *entry);

berr  acr_api_rule_account_del(acr_account_rule_t *entry);


acr_account_rule_t *acr_api_rule_account_show(char *account);


/* dp use */
berr acr_dp_api_match(uint8_t *account, uint16_t len);

berr acr_dp_api_init();

berr acr_dp_api_request_data_entry(void);


#endif
