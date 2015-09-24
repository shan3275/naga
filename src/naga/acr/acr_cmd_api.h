/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/acr\acr_cmd_api.h
#       @date         :2015/08/21 22:54
#       @algorithm    :
=============================================================================*/



#ifndef __ACR_CMD_API_H__
#define __ACR_CMD_API_H__


#include "acr_account_rule.h"
#include "boots.h"




berr rule_acr_cmd_add_account(acr_account_rule_t *entry);

berr rule_acr_cmd_del_account(acr_account_rule_t *entry);

acr_account_rule_t *rule_acr_cmd_show_account(char *account);


#endif /*__ACR_CMD_API_H__ */
