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
#include "naga_types.h"
#include "naga_host_rule.h"
#include "dmr_cmd_api.h"

#include "version.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#define RULE_STR                    "Access control list\n"
#define DMR_STR						"DMR module\n"
#define ADD_STR                     "ADD Operation\n"
#define DEL_STR                     "Del Operation\n"
//#define SHOW_STR                    "Show entry content\n"
#define FLUSH_STR                   "Flush url\n"
//#define CLEAR_STR                   "Clear Operation\n"
#define STAT_STR                    "Statistics Operation\n"
#define ENCOURAGE_STR               "Encourage Operation, for test\n" 
#define TOTAL_STR                   "Summary Display\n"

#define HOST_STR                    "Opearation of http host\n"
#define DMR_HOST_STR                "Http host\n"
#define ACTION_STR                  "forward or drop\n"
#define LOAD_STR                    "load host file\n"
#define HOST_FILE_STR               "Host file to be loaded\n"



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
                return E_FAIL;
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
                return E_FAIL;
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
                return E_FAIL;
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
                return E_FAIL;
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
                return E_FAIL;
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
                return E_FAIL;
            }
            
            *cnt = 5;
            break;
        case 'i': /* ignore case , must a-z|A-Z */
            if ((keyword[2] | 0x20) > 0x7a || (keyword[2] | 0x20) < 0x61) 
            {
                return E_FAIL;
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
            return E_FAIL;
    }
    
    return E_SUCCESS;
}


static berr host_str2bit(const char *str, uint8_t *data, uint32_t *len)
{
	uint32_t i = 0, j = 0;
    uint32_t cnt = 0;

    if ((str == NULL) || (data == NULL) || (len == NULL))
    {
        return E_FAIL;
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
	                return E_FAIL;
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









static int dmr_cmd_show_host(struct vty *vty, const char *host)
{

	int ret = 0;
	uint8_t data[MAX_HOST_LEN];
	uint32_t len;
	naga_host_attr_t host_attr;
	naga_host_rule_t *entry;
	
	memset(&host_attr, 0, sizeof(naga_host_attr_t));
	
	if (NULL == host)
	{
        return CMD_ERR_NO_MATCH;
	}
	
	if (host_str2bit(host, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}

	host_attr.host_len = len;
	memcpy(host_attr.host, data, len);
	

    entry = rule_dmr_cmd_show_host(&host_attr);
    if (NULL == entry)
    {
        vty_out(vty, "dmr del host, %s ret(%d)%s", host, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}




DEFUN(dmr_show_host,
      dmr_show_host_cmd,
      "rule dmr host show HOST",
      RULE_STR
      DMR_STR
      HOST_STR
      SHOW_STR
      DMR_HOST_STR)
{
    return dmr_cmd_show_host(vty, argv[0]);
}




static int dmr_cmd_del_host(struct vty *vty, const char *host)
{
    int ret = 0;
	uint8_t data[MAX_HOST_LEN];
	uint32_t len;
	naga_host_rule_t entry;
	
	memset(&entry, 0, sizeof(naga_host_rule_t));
	
	if (NULL == host)
	{
        return CMD_ERR_NO_MATCH;
	}
#if 0
	if (host_str2bit(host, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}
#endif
	len = strlen(host);
	entry.host.host_len = len;
	memcpy(entry.host.host, data, len);
	

    ret = rule_dmr_cmd_del_host(&entry);
    if (ret)
    {
        vty_out(vty, "dmr del host, %s ret(%d)%s", host, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}





DEFUN(dmr_del_host, 
      dmr_del_host_cmd,
      "rule dmr host del HOST",
      RULE_STR
      DMR_STR
      HOST_STR
      DEL_STR
      DMR_HOST_STR)
{
    return dmr_cmd_del_host(vty, argv[0]);
}





static int dmr_cmd_add_host(struct vty *vty, const char *host, const char *action_str)
{
    int ret = 0;
    uint32_t  action = 0, len = 0;
	//uint8_t data[MAX_HOST_LEN];

	naga_host_rule_t entry;
	
	memset(&entry, 0, sizeof(naga_host_rule_t));
		
	if ((NULL == host) || (NULL == action_str))
	{
        return CMD_ERR_NO_MATCH;
	}
	
    //index  = atoi(index_str);
	action_str2int(action_str, &action);
#if 0
	if (host_str2bit(host, data, &len))
	{
		return CMD_ERR_NO_MATCH;
	}
#endif
	len = strlen(host);
	entry.acl.actions = action;
	entry.host.host_len = len;
	memcpy(entry.host.host, host, len);

	printf("host = %s, host_len = %d\n", host, len);

    ret = rule_dmr_cmd_add_host(&entry);
    if (ret)
    {
        vty_out(vty, "dmr add host host(%s) ret(%d)%s", host, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}





/*host operation*/

DEFUN(dmr_add_host, 
      dmr_add_host_cmd,
      "rule dmr host add HOST {push|drop}",
      RULE_STR
      DMR_STR
      HOST_STR
      ADD_STR
      DMR_HOST_STR
      ACTION_STR)
{
    return dmr_cmd_add_host(vty, argv[0], argv[1]);
}



static int dmr_cmd_load_host(struct vty *vty, const char *file_name)
{
	FILE *fp = NULL;
	char host_line[MAX_HOST_LEN] = {0};
	int rv = 0;

	fp = fopen(file_name, "+r");
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

		rv = dmr_cmd_add_host(vty, host_line, "push");
		if (CMD_SUCCESS != rv)
		{
			dmr_debug("Add host %s rule failed!\n", host_line);
			return rv;
		}
	}

	fclose(fp);

	return CMD_SUCCESS;
}


DEFUN(dmr_load_host_cfg,
      dmr_load_host_cfg_cmd,
      "rule dmr load host FILE",
      RULE_STR
      DMR_STR
      LOAD_STR
      HOST_STR
      SHOW_STR
      HOST_FILE_STR)
{
    return dmr_cmd_load_host(vty, argv[0]);
}




/*
 * dmr module cmdline register and init
 *
 * */
void cmdline_dmr_init(void)
{
	install_element(CMD_NODE, &dmr_add_host_cmd);
	install_element(CMD_NODE, &dmr_del_host_cmd);
	install_element(CMD_NODE, &dmr_show_host_cmd);
	install_element(CMD_NODE, &dmr_load_host_cfg_cmd);

    return ;
}
