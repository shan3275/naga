/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/acr\acr_cmd_api.c
#       @date         :2015-08-22 19:38
#       @algorithm    :
=============================================================================*/

//#include "acr_cmd_api.h"
#include "acr_api.h"
#include "acr_account_rule.h"
#include "boots.h"

berr rule_acr_cmd_add_account(acr_account_entry_t *entry)
{
	return acr_api_rule_account_add(entry);
}

berr rule_acr_cmd_del_account(acr_account_entry_t *entry)
{
	return acr_api_rule_account_del(entry);
}


acr_account_entry_t *rule_acr_cmd_show_account(char *account)
{
	return acr_api_rule_account_show(account);
}




