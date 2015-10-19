/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :gongjianhua
#       @name         :Sam Liu
#       @file         :/home/shan/work/rose/src/naga/vsr\vsr_cmd.c
#       @date         :2015/09/05 11:53
#       @algorithm    :
=============================================================================*/
#include <string.h>

#include "zebra.h"
#include "getopt.h"
#include "command.h"

#include "boots.h"
#include "bts_debug.h"

#include "naga_types.h"
#include "naga_util.h"
#include "naga_cmd.h"

#include "dmr.h"

#include "version.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#define DOMAIN_STR					"Domain rule\n"
#define DOMAIN_NAME_STR             "Name of domain\n" 
#define DOMAIN_ALL_STR             "All the domain\n"


#define DMR_DEBUG
#ifdef  DMR_DEBUG
#define dmr_debug(fmt,args...)  vty_out(vty, "func=%s line=%d:" fmt "%s", __FUNCTION__, __LINE__, ##args, VTY_NEWLINE)
#else
#define dmr_debug(fmt,args...)
#endif  /* DEBUG */

static berr convertChar(const char *keyword, uint8_t *data, uint32_t *cnt)
{
    uint8_t temp = 0;
    
    switch (keyword[1]) 
    {
        case 'x': /* \xff */
            temp = 0;
            if (keyword[2] > 0x2f && keyword[2] < 0x3a) 
            {
                /* ascii 0-9 */
                temp += 0x10 * (keyword[2] - 0x30);
            } 
            else if ((keyword[2] | 0x20) > 0x60 && (keyword[2] | 0x20) < 0x67)
            {
                /* ascii a-f, A-F */
                temp += 0x10 * ((keyword[2] | 0x20) - 0x61 + 10);
            } 
            else 
            {
                BRET(E_FAIL);
            }
                
            if (keyword[3] > 0x2f && keyword[3] < 0x3a) 
            {
                /* ascii 0-9 */
                temp += (keyword[3] - 0x30);
            } 
            else if ((keyword[3] | 0x20) > 0x60 && (keyword[3] | 0x20) < 0x67) 
            {
                /* ascii a-f, A-F */
                temp += ((keyword[3] | 0x20) - 0x61 + 10);
            } 
            else 
            {
                BRET(E_FAIL);
            }
            
            *data = temp;
            *cnt = 3;
            break;
        case 'm': /* \xddmm , one byte data one byte mask */
            temp = 0;
            if (keyword[2] > 0x2f && keyword[2] < 0x3a) 
            {
                /* ascii 0-9 */
                temp += 0x10 * (keyword[2] - 0x30);
            } 
            else if ((keyword[2] | 0x20) > 0x60 && (keyword[2] | 0x20) < 0x67) 
            {
                /* ascii a-f, A-F */
                temp += 0x10 * ((keyword[2] | 0x20) - 0x61 + 10);
            } 
            else 
            {
                BRET(E_FAIL);
            }
            
            if (keyword[3] > 0x2f && keyword[3] < 0x3a) 
            {
                /* ascii 0-9 */
                temp += (keyword[3] - 0x30);
            } 
            else if ((keyword[3] | 0x20) > 0x60 && (keyword[3] | 0x20) < 0x67) 
            {
                /* ascii a-f, A-F */
                temp += ((keyword[3] | 0x20) - 0x61 + 10);
            } 
            else 
            {
                BRET(E_FAIL);
            }
            
            *data = temp;
            temp = 0;
            
            if (keyword[4] > 0x2f && keyword[4] < 0x3a) 
            {
                /* ascii 0-9 */
                temp += 0x10 * (keyword[4] - 0x30);
                //printf("%s.%d: keyword[4]=%c\n", __func__, __LINE__, keyword[4]);
            } 
            else if ((keyword[4] | 0x20) > 0x60 && (keyword[4] | 0x20) < 0x67) 
            {
                /* ascii a-f, A-F */
                temp += 0x10 * ((keyword[4] | 0x20) - 0x61 + 10);
                //printf("%s.%d: keyword[4]=%c\n", __func__, __LINE__, keyword[4]);
            } 
            else 
            {
                BRET(E_FAIL);
            }
            
            if (keyword[5] > 0x2f && keyword[5] < 0x3a) 
            {
                /* ascii 0-9 */
                temp += (keyword[5] - 0x30);
                //printf("%s.%d: keyword[5]=%c\n", __func__, __LINE__, keyword[5]);
            } 
            else if ((keyword[5] | 0x20) > 0x60 && (keyword[5] | 0x20) < 0x67) 
            {
                /* ascii a-f, A-F */
                temp += ((keyword[5] | 0x20) - 0x61 + 10);
                //printf("%s.%d: keyword[5]=%c\n", __func__, __LINE__, keyword[5]);
            } 
            else 
            {
                BRET(E_FAIL);
            }
            
            *cnt = 5;
            break;
        case 'i': /* ignore case , must a-z|A-Z */
            if ((keyword[2] | 0x20) > 0x7a || (keyword[2] | 0x20) < 0x61) 
            {
                BRET(E_FAIL);
            }
            
            *data = keyword[2];
            *cnt = 2;
            break;
        case '*':
            *data = '*';
            *cnt = 1;
            break;
        case 's':
            *data = ' ';
            *cnt = 1;
            break;
        case '\\':
            *data = '\\';
            *cnt = 1;
            break;
        case 'r':
            *data = '\r';
            *cnt = 1;
            break;
        case 'n':
            *data = '\n';
            *cnt = 1;
            break;
        case 't':
            *data = '\t';
            *cnt = 1;
            break;
        case '0':
            *data = 0x0;
            *cnt = 1;
            break;
        case 'b':
            *data = 0x8;
            *cnt = 1;
            break;
        case 'a':
            *data = 0x7;
            *cnt = 1;
            break;
        case 'q':
            *data = '?';
            *cnt = 1;
            break;
        case 'S':
            *data = '\'';
            *cnt = 1;
            break;

        default:
            BRET(E_FAIL);
    }
    
    return E_SUCCESS;
}


static berr host_str2bit(const char *str, uint8_t *data, uint32_t *len)
{
	uint32_t i = 0, j = 0;
    uint32_t cnt = 0;

    if ((str == NULL) || (data == NULL) || (len == NULL))
    {
        BRET(E_FAIL);
    }

    for (i = 0; i < MAX_HOST_LEN; i++, j++) 
    {
	    switch (str[i]) 
	    {
	        case '\0':
	        case '\n':
	        case '\r':
	        case '\t':
	            return E_SUCCESS;
	        case '\\':
	            if (convertChar(&str[i], &data[j], &cnt) != E_SUCCESS)
	            {              
	                BRET(E_FAIL);
	            }
	            
	            i += cnt; /* skip character */
	            break;
	        case '*':
	            data[j] = 0x0;
	            break;
	        /*case ' ':  //unsupport input space(' ') keyword
	            return ERROR;*/
	        case ' ':   //support input space(' ') keyword
	            data[j] = ' ';
	            break;
	        case '?':
	            data[j] = '?';
	            break;
	        default:
	            data[j] = (uint8_t)str[i];
	            break;
	    }
    	
        *len = j;
    }

    /*Check Input strlen out of range*/
    if (str[i] != '\0')
    {
        return  E_FAIL;
    }
    return E_SUCCESS;

}

static dmr_t *dmr_entry_new(void)
{
	dmr_t *entry = NULL;

	entry = malloc(sizeof(dmr_t));
	if (NULL != entry)
	{
		memset(entry, 0, sizeof(dmr_t));
	}

	return entry;
}

void
dmr_dump_vty(void *data, void *param)
{
    dmr_t *entry = NULL;
    struct vty *vty = NULL;

    char action_str[NAGA_ACTION_STR_SZ];

    if ((NULL == param) || (NULL == data))
    {
        return;
    }

    entry = (dmr_t *) data;
    vty   = (struct vty *) param;

    naga_action_string(&entry->acl.actions, action_str);

    vty_out(vty, "%-32s %-32s %-16ld %s", entry->host, action_str, 
            (uint64_t) entry->acl.cnt.cnt, VTY_NEWLINE);
}

static int cmd_dmr_show(struct vty *vty, const char *host)
{
	int ret = 0;

	dmr_t *entry = NULL;

	if (NULL == host)
	{
        return CMD_ERR_NOTHING_TODO;
	}

    entry = api_dmr_get((char *)host);
    if (NULL == entry)
    {
        vty_out(vty, "dmr host<%s> empty%s", host, VTY_NEWLINE);
        return CMD_WARNING;
    }
	vty_out(vty, "%-32s %-32s %-16s %s","host","action", "cnt",VTY_NEWLINE);
    vty_out(vty, "------------------------------------------------%s", VTY_NEWLINE);

    dmr_dump_vty((void *)entry, (void *)vty);
   
    return CMD_SUCCESS;
}


static int cmd_dmr_show_all(struct vty *vty)
{
	vty_out(vty, "%-32s %-32s %-16s %s","host","action", "cnt",VTY_NEWLINE);
    vty_out(vty, "------------------------------------------------%s", VTY_NEWLINE);
	
    dmr_iter(dmr_dump_vty, vty);

    return CMD_SUCCESS;
}




DEFUN(show_domain,
      show_domain_cmd,
      "show domain NAME",
      SHOW_STR
      DOMAIN_STR
      DOMAIN_NAME_STR)
{
    return cmd_dmr_show(vty, argv[0]);
}


DEFUN(show_domain_all,
      show_domain_all_cmd,
      "show domain all",
      SHOW_STR
      DOMAIN_STR
      DOMAIN_ALL_STR)
{
    return cmd_dmr_show_all(vty);
}



static int cmd_dmr_del(struct vty *vty, const char *host)
{
    int ret = 0;
	
	if (NULL == host)
	{
        return CMD_ERR_NOTHING_TODO;
	}

    ret = api_dmr_del((char *)host);

    if (ret)
    {
        vty_out(vty, "dmr del  %s fail:(%s)%s", host, berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}


static int cmd_dmr_del_all(struct vty *vty)
{
	int ret = 0;
	ret = api_dmr_clear();
	if (ret)
    {
        vty_out(vty, "dmr del all domain fail:(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}


DEFUN(remove_domain, 
      remove_domain_cmd,
      "remove domain NAME",
      REMOVE_STR
      DOMAIN_STR
      DOMAIN_NAME_STR)
{
    return cmd_dmr_del(vty, argv[0]);
}

DEFUN(remove_domain_all, 
      remove_domain_all_cmd,
      "remove domain all",
      REMOVE_STR
      DOMAIN_STR
      DOMAIN_ALL_STR)
{
    return cmd_dmr_del_all(vty);
}


static int cmd_dmr_add(struct vty *vty, const char *host, const char *action_str)
{
    berr ret = 0;
	char action_arry[NAGA_ACTION_STR_SZ]= {0};
	dmr_t *entry = NULL;
		
	if ((NULL == host) || (NULL == action_str))
	{
        return CMD_ERR_NO_MATCH;
	}

    entry = dmr_entry_new();
	if (NULL == entry)
	{
        return CMD_ERR_NO_MATCH;
	}

    sprintf(action_arry, "%s", action_str);
	if(naga_action_parse(action_arry, &entry->acl.actions))
    {
        return CMD_ERR_NO_MATCH;
    }
	
	entry->host_len = strlen(host);
	memcpy(entry->host, host, entry->host_len);

    ret = api_dmr_add(entry);
    if (ret)
    {
        vty_out(vty, "dmr add host fail: ret(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}

/*host operation*/

DEFUN(domain, 
      domain_cmd,
      "domain NAME ACT",
      DOMAIN_STR
      DOMAIN_NAME_STR
      ACTION_STR)
{
    return cmd_dmr_add(vty, argv[0], argv[1]);
}

static int cmd_dmr_load(struct vty *vty, const char *file_name, const char *action_str)
{
	FILE *fp = NULL;
	char host_line[MAX_HOST_LEN] = {0};
	int rv = 0;
	char *p = NULL;

	fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		dmr_debug("Open the file %s failed!\n", file_name);
		return CMD_ERR_NOTHING_TODO;
	}

	while(NULL != fgets(host_line, MAX_HOST_LEN, fp))
	{
		if ('#' == host_line[0])
		{
			continue;
		}

		if (NULL != (p = strchr(host_line, '\n')))
		{
            *p = '\0';
		}	

		rv = cmd_dmr_add(vty, host_line, action_str);
		if (CMD_SUCCESS != rv)
		{
			dmr_debug("Add host %s rule failed!\n", host_line);
			return rv;
		}
	}

	fclose(fp);

	return CMD_SUCCESS;
}


DEFUN(load_domain,
      load_domain_cmd,
      "load domain FILE ACT",
      LOAD_STR
      DOMAIN_STR
      FILE_STR
      ACTION_STR)
{
    return cmd_dmr_load(vty, argv[0], argv[1]);
}

static int cmd_dmr_clear_stat(struct vty *vty, const char *host)
{
	int ret = 0;

    if (NULL == host)
    {
        return CMD_ERR_NOTHING_TODO;
    }
	
    ret = api_dmr_stat_clear((char *)host); 
	if (ret)
    {
        vty_out(vty, "dmr clear domain %s rule stat fail:(%s)%s", host, berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;

}


static int cmd_dmr_clear_stat_all(struct vty *vty)
{
	int ret = 0;
	ret = api_dmr_stat_clear_all();
	if (ret)
    {
        vty_out(vty, "dmr clear all domain rule stat fail:(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}


DEFUN(clear_domain_stat,
      clear_domain_stat_cmd,
      "clear domain stat DOMAIN",
      CLEAR_STR
      DOMAIN_STR
      STAT_STR
      DOMAIN_STR)
{
    return cmd_dmr_clear_stat(vty, argv[0]);
}





DEFUN(clear_domain_stat_all,
      clear_domain_stat_all_cmd,
      "clear domain stat all",
      CLEAR_STR
      DOMAIN_STR
      STAT_STR
      DOMAIN_ALL_STR)
{
    return cmd_dmr_clear_stat_all(vty);
}


static int cmd_dmr_domain_default_act_set(struct vty *vty, const char *act_str)
{
	int ret = 0;
	uint32_t action = 0;
	
	if(naga_action_parse((char *)act_str, &action))
    {
        return CMD_ERR_NO_MATCH;
    }
	ret = api_dmr_domain_default_act_set(action);
	if (ret)
    {
        vty_out(vty, "domain set default action fail:(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}



DEFUN(domain_default_act_set,
      domain_default_act_set_cmd,
      "domain default ACT",
      DOMAIN_STR
      DEFAULT_STR
      ACTION_STR)
{
    return cmd_dmr_domain_default_act_set(vty, argv[0]);
}








/*
 * dmr module cmdline register and init
 *
 * */







void
dmr_write_config_vty(void *data, void *param)
{
    dmr_t *entry = NULL;

    struct vty *vty = NULL;

    char action_str[NAGA_ACTION_STR_SZ];

    if ((NULL == param) || (NULL == data))
    {
        return;
    }

    entry = (dmr_t *) data;
    vty   = (struct vty *) param;

    naga_action_string(&entry->acl.actions, action_str);
    vty_out(vty, "domain %s %s %s", entry->host, action_str, VTY_NEWLINE);
	
}



void dmr_cmd_config_write(struct vty *vty)
{
	int ret = 0;
	uint32_t action = 0;
	char action_str[NAGA_ACTION_STR_SZ] = {0};
	dmr_iter(dmr_write_config_vty, vty);
	
	ret = api_dmr_domain_default_act_get(&action);
	if (ret)
    {
        vty_out(vty, "domain get default action fail:(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return;
    }
	naga_action_string(&action, action_str);
	vty_out(vty, "domain default %s%s", action_str, VTY_NEWLINE);
	
	 
}



void cmdline_dmr_init(void)
{
	install_element(CMD_NODE, &domain_cmd);
	install_element(CMD_NODE, &remove_domain_cmd);
	install_element(CMD_NODE, &remove_domain_all_cmd);
	install_element(CMD_NODE, &load_domain_cmd);
	install_element(CMD_NODE, &clear_domain_stat_cmd);
	install_element(CMD_NODE, &clear_domain_stat_all_cmd);
	install_element(CMD_NODE, &show_domain_cmd);
	install_element(CMD_NODE, &show_domain_all_cmd);
	install_element(CMD_NODE, &domain_default_act_set_cmd);	

    return ;
}
