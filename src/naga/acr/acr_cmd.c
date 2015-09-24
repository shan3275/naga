/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/home/shan/work/rose/src/naga/acr\acr_cmd.c
#       @date         :2015/09/05 11:53
#       @algorithm    :
=============================================================================*/
#include <string.h>

#include "acr_cmd_api.h"
#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"
#include "acr_account_rule.h"

#define RULE_STR                    "Access control list\n"
#define ACR_STR                     "Vister Record list\n"
#define ADD_STR                     "ADD Operation\n"
#define DEL_STR                     "Del Operation\n"
//#define SHOW_STR                    "Show entry content\n"

#define ACR_ALL_STR                 "All Vister Record list\n"

//#define CLEAR_STR                   "Clear Operation\n"
#define STAT_STR                    "Statistics Operation\n"
#define ACCOUNT_STR                 "account info\n"
#define ACCOUNT_DATA_STR            "account data\n"
#define ACTION_STR                  "forward or drop\n"



//#define DEBUG
#ifdef  DEBUG
#define acr_debug(fmt,args...)  vty_out(vty, "func=%s line=%d:" fmt "%s", __FUNCTION__, __LINE__, ##args, VTY_NEWLINE)
#else
#define acr_debug(fmt,args...)
#endif  /* DEBUG */



static int acr_cmd_add_account(struct vty *vty, const char *account, const char *action_str)
{
    int ret = 0;
    uint32_t  action = 0;

	acr_account_rule_t entry;
	
	memset(&entry, 0, sizeof(acr_account_rule_t));
		
	if ((NULL == account) || (NULL == action_str))
	{
        return CMD_ERR_NO_MATCH;
	}
	
	action_str2int(action_str, &action);

	entry.acl.actions = action;
	memcpy(entry.account, account, strlen(account));

    ret = rule_acr_cmd_add_account(&entry);
    if (ret)
    {
        vty_out(vty, "acr add account account(%s) ret(%d)%s", account, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}





/*account operation*/

DEFUN(acr_add_account, 
      acr_add_account_cmd,
      "rule acr account add ACCOUNT {push|drop}",
      RULE_STR
      ACR_STR
      ACCOUNT_STR
      ADD_STR
      ACCOUNT_DATA_STR
      ACTION_STR)
{
    return acr_cmd_add_account(vty, argv[0], argv[1]);
}



static int acr_cmd_del_account(struct vty *vty, const char *account)
{
    int ret = 0;
	acr_account_rule_t entry;
	
	memset(&entry, 0, sizeof(acr_account_rule_t));
	
	if (NULL == account)
	{
        return CMD_ERR_NO_MATCH;
	}

	memcpy(entry.account, account, strlen(account));
	

    ret = rule_acr_cmd_del_account(&entry);
    if (ret)
    {
        vty_out(vty, "acr del account, %s ret(%d)%s", account, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}





DEFUN(acr_del_account, 
      acr_del_account_cmd,
      "rule acr account del ACCOUNT",
      RULE_STR
      ACR_STR
      ACCOUNT_STR
      DEL_STR
      ACCOUNT_DATA_STR)
{
    return acr_cmd_del_account(vty, argv[0]);
}



static int acr_cmd_show_account(struct vty *vty, const char *account)
{
	char data[NAGA_ACCOUNT_STR_SZ] = {0};
	acr_account_rule_t *entry;
	
	if (NULL == account)
	{
        return CMD_ERR_NO_MATCH;
	}

	memcpy(data, account, strlen(account));

    entry = rule_acr_cmd_show_account(data);
    if (NULL == entry)
    {
        vty_out(vty, "acr del account, %s %s", account, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}




DEFUN(acr_show_account,
      acr_show_account_cmd,
      "rule acr account show ACCOUNT",
      RULE_STR
      ACR_STR
      ACCOUNT_STR
      SHOW_STR
      ACCOUNT_DATA_STR)
{
    return acr_cmd_show_account(vty, argv[0]);
}




/*
 * acr module cmdline register and init 
 *
 * */
void cmdline_acr_init(void)
{

    install_element(CMD_NODE, &acr_add_account_cmd);

    install_element(CMD_NODE, &acr_del_account_cmd);
    //install_element(CMD_NODE, &acr_del_all_cmd);

    install_element(CMD_NODE, &acr_show_account_cmd);
    //install_element(CMD_NODE, &acr_show_by_ip_cmd);
	
    return ;
}
