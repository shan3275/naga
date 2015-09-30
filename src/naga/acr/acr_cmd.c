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


#define ACCOUNT_STR                    "Opearation of http host\n"
#define ACR_ACCOUNT_STR                "Http host\n"
#define ACTION_STR                  "forward or drop\n"
#define LOAD_STR                    "load host file\n"
#define ACCOUNT_FILE_STR               "Host file to be loaded\n"
#define ALL_STR                     "All rules\n"
#define ACR_STAT_STR                    "acr rule stat\n" 



//#define DEBUG
#ifdef  DEBUG
#define acr_debug(fmt,args...)  vty_out(vty, "func=%s line=%d:" fmt "%s", __FUNCTION__, __LINE__, ##args, VTY_NEWLINE)
#else
#define acr_debug(fmt,args...)
#endif  /* DEBUG */



static acr_account_rule_t *acr_account_entry_new(void)
{
	acr_account_rule_t *entry = NULL;

	entry = malloc(sizeof(acr_account_rule_t));
	if (NULL != entry)
	{
		memset(entry, 0, sizeof(acr_account_rule_t));
	}

	return entry;
}







static int acr_cmd_show_account(struct vty *vty, const char *account)
{

	int ret = 0;
	uint32_t len = 0;
	acr_account_rule_t *entry;
	acr_account_rule_t data;
	//memset(&account_info, 0, sizeof(acr_account_rule_t));
	
	if (NULL == account)
	{
        return CMD_ERR_NO_MATCH;
	}
	#if 0
	if (account_str2bit(account, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}
#endif
	memset(&data, 0, sizeof(acr_account_rule_t));

	len = strlen(account);
	data.account_len = len;
	memcpy(data.account, account, len);

	vty_out(vty, "%-32s %-16s %-16s %s","account","action", "cnt",VTY_NEWLINE);
	vty_out(vty, "---------------------------------%s", VTY_NEWLINE);

    entry = rule_acr_cmd_show_account(&data);
    if (NULL == entry)
    {
        vty_out(vty, "account %s empty%s", account, VTY_NEWLINE);
        return CMD_WARNING;
    }

	vty_out(vty, "%-32s %-16d %-16ld %s", entry->account, entry->acl.actions, 
		(uint64_t)entry->acl.cnt.cnt, VTY_NEWLINE);

    return CMD_SUCCESS;
}




DEFUN(acr_show_account,
      acr_show_account_cmd,
      "rule acr account show ACCOUNT",
      RULE_STR
      ACR_STR
      ACCOUNT_STR
      SHOW_STR
      ACR_ACCOUNT_STR)
{
    return acr_cmd_show_account(vty, argv[0]);
}




static int acr_cmd_del_account(struct vty *vty, const char *account)
{
    int ret = 0;
	uint32_t len;
	acr_account_rule_t entry;
	
	memset(&entry, 0, sizeof(acr_account_rule_t));
	
	if (NULL == account)
	{
        return CMD_ERR_NO_MATCH;
	}
#if 0
	if (account_str2bit(account, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}
#endif
	len = strlen(account);
	entry.account_len = len;
	memcpy(entry.account, account, len);

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
      ACR_ACCOUNT_STR)
{
    return acr_cmd_del_account(vty, argv[0]);
}



static int acr_cmd_del_all(struct vty *vty)
{
	int ret = 0;
	ret = rule_acr_cmd_del_all();
	if (ret)
    {
        vty_out(vty, "acr delete all account failed! ret(%d)%s", ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
	
}

DEFUN(acr_del_all_account, 
      acr_del_all_account_cmd,
      "rule acr account del all",
      RULE_STR
      ACR_STR
      ACCOUNT_STR
      DEL_STR
      ALL_STR)
{

    return acr_cmd_del_all(vty);
}





static int acr_cmd_add_account(struct vty *vty, const char *account, const char *action_str)
{
    int ret = 0;
    uint32_t  action = 0, len = 0;
	//suint8_t data[MAX_ACCOUNT_LEN];

	acr_account_rule_t *entry;
		
	if ((NULL == account) || (NULL == action_str))
	{
        return CMD_ERR_NO_MATCH;
	}

	entry = acr_account_entry_new();
	if (NULL == entry)
	{
		return CMD_ERR_NO_MATCH;
	}
	
    //index  = atoi(index_str);
	action_str2int(action_str, &action);
#if 0
	if (account_str2bit(account, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}
#endif

	
	len = strlen(account);
	entry->acl.actions = action;
	entry->account_len = len;
	memcpy(entry->account, account, len);

	printf("account = %s, account_len = %d\n", account, len);

    ret = rule_acr_cmd_add_account(entry);
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
      ACR_ACCOUNT_STR
      ACTION_STR)
{
    return acr_cmd_add_account(vty, argv[0], argv[1]);
}



static int acr_cmd_load_account(struct vty *vty, const char *file_name)
{
	FILE *fp = NULL;
	char account_line[NAGA_ACCOUNT_STR_SZ] = {0};
	int rv = 0;
	char *p = NULL;

	fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		acr_debug("Open the file %s failed!\n", file_name);
		return CMD_ERR_NOTHING_TODO;
	}

	while(NULL != fgets(account_line, NAGA_ACCOUNT_STR_SZ, fp))
	{
		if ('#' == account_line[0])
		{
			continue;
		}

		if (NULL != (p = strchr(account_line, '\n')))
		{
            *p = '\0';
		}

		rv = acr_cmd_add_account(vty, account_line, "push");
		if (CMD_SUCCESS != rv)
		{
			acr_debug("Add account %s rule failed!\n", account_line);
			return rv;
		}
	}

	fclose(fp);

	return CMD_SUCCESS;
}


DEFUN(acr_load_account_cfg,
      acr_load_account_cfg_cmd,
      "rule acr load account FILE",
      RULE_STR
      ACR_STR
      LOAD_STR
      ACCOUNT_STR
      SHOW_STR
      ACCOUNT_FILE_STR)
{
    return acr_cmd_load_account(vty, argv[0]);
}




static int acr_cmd_clear_account_stat(struct vty *vty, const char *account)
{
	int ret = 0;
	uint32_t len;
	acr_account_rule_t *entry;
	acr_account_rule_t data;
	
	memset(&data, 0, sizeof(acr_account_rule_t));
	
	if (NULL == account)
	{
		return CMD_ERR_NO_MATCH;
	}
#if 0
	if (account_str2bit(account, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}
#endif
	len = strlen(account);
	data.account_len = len;
	memcpy(data.account, account, len);

	entry = rule_acr_cmd_show_account(&data);
	if (NULL == entry)
	{
		vty_out(vty, "This account %s rule does not exist%s", account, VTY_NEWLINE);
		return CMD_ERR_NOTHING_TODO;
	}

	ACL_CNT_CLEAR(entry->acl);

    return CMD_SUCCESS;

}




DEFUN(acr_clear_account_rule_stat,
      acr_clear_account_rule_stat_cmd,
      "rule acr clear account stat ACCOUNT",
      RULE_STR
      ACR_STR
      CLEAR_STR
      ACCOUNT_STR
      ACR_STAT_STR
      ACR_ACCOUNT_STR)
{
    return acr_cmd_clear_account_stat(vty, argv[0]);
}



/*
 * acr module cmdline register and init
 *
 * */
void cmdline_acr_init(void)
{
	install_element(CMD_NODE, &acr_add_account_cmd);
	install_element(CMD_NODE, &acr_del_all_account_cmd);
	install_element(CMD_NODE, &acr_del_account_cmd);
	install_element(CMD_NODE, &acr_show_account_cmd);
	install_element(CMD_NODE, &acr_load_account_cfg_cmd);
	install_element(CMD_NODE, &acr_clear_account_rule_stat_cmd);

    return ;
}

