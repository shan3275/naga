
/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/home/shan/work/rose/src/naga/vsr\net_cmd.c
#       @date         :2015/09/05 11:53
#       @algorithm    :
=============================================================================*/
#include <string.h>

#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
//#include "prefix.h"
#include "privs.h"
//#include "naga_util.h"
//#include "bts_util.h"
#include "hijack.h"
#include "bts_cnt.h"

#define HIJACK_IP_STR               "ip what refers users\n"
#define INTERVAL_STR                "set the percent\n"
#define HIJACK_STR                  "hajack settings"
#define ADD_STR                     "ADD Operation\n"
#define DEL_STR                     "Del Operation\n"
#define HIJACK_INDEX                "0-49\n"
#define HIJACK_ALL                  "All hijack rules\n"

//#define DEBUG
#ifdef  DEBUG
#define hj_debug(fmt,args...)  vty_out(vty, "func=%s line=%d:" fmt "%s", __FUNCTION__, __LINE__, ##args, VTY_NEWLINE)
#else
#define hj_debug(fmt,args...)
#endif  /* DEBUG */

static int ip_num_interval(const char *interval_str)
{
    int interval = strtoul(interval_str, NULL, 0);
    return api_ip_num_set_interval(interval);
}

DEFUN(ip_num_interval_set,
      ip_num_interval_set_cmd,
      "hijack ip number interval <1-100000>",
      HIJACK_STR
      HIJACK_IP_STR
      "ip number\n"
      INTERVAL_STR
      "1-100000\n"
      )
{
    return ip_num_interval(argv[0]);
}


static int ip_interval(const char *interval_str)
{
    int interval = strtoul(interval_str, NULL, 0);
    return api_ip_set_interval(interval);
}

DEFUN(ip_interval_set,
      ip_interval_set_cmd,
      "hijack ip interval <1-100000>",
      HIJACK_STR
      HIJACK_IP_STR
      INTERVAL_STR
      "1-100000\n"
      )
{
    return ip_interval(argv[0]);
}


static int hijack_info_get(struct vty *vty)
{
    int status = 0;
    int ip_interval = 0, ip_num_interval = 0;
    uint64_t hijack_can_push_count;
    uint64_t hijack_push_tx_success;
    time_t *boottime = NULL;

    if (api_hijack_enable_get(&status))
    {
        vty_out(vty, "Get hijack switch fail!%s", VTY_NEWLINE);
        return CMD_WARNING;
    }
    if (api_ip_interval_get(&ip_interval))
    {
        vty_out(vty, "Get hijack ip interval fail!%s", VTY_NEWLINE);
        return CMD_WARNING;
    }
    if (api_ip_num_interval_get(&ip_num_interval))
    {
        vty_out(vty, "Get hijack ip number interval fail!%s", VTY_NEWLINE);
        return CMD_WARNING;
    }

    vty_out(vty, "Hajack module is: %s%s", status==1?"ON":"OFF", VTY_NEWLINE);
    vty_out(vty, "ip interval is: %d%s", ip_interval, VTY_NEWLINE);
    vty_out(vty, "ip number interval is: %d%s", ip_num_interval, VTY_NEWLINE);

    hijack_can_push_count = CNT_GET(HIJACK_ALL_CAN_PUSH); 
    hijack_push_tx_success = CNT_GET(HIJACK_PUSH_TX_SUCCESS);

    boottime = api_hijack_get_start_time();

    if(boottime !=  NULL)
    {
        vty_out(vty, "Boot Time      : %s%s", ctime(boottime), VTY_NEWLINE);
    }

    vty_out(vty, "All can hijack          : %ld%s",  hijack_can_push_count, VTY_NEWLINE);    
    vty_out(vty, "Pushed(tx success)    : %ld%s",  hijack_push_tx_success, VTY_NEWLINE);


    return CMD_SUCCESS;
}

DEFUN(show_hijack_interval,
      show_hijack_interval_cmd,
      "show hijack info",
      SHOW_STR
      HIJACK_STR
      "infomation for hijack\n"
      )
{
    return hijack_info_get(vty);
}



static int hijack_cmd_enable_set(struct vty *vty, const char *enable)
{
    int status = 0;
    if (!strcmp("on", enable))
    {
        status = 1;
    }
    else
    {
        status = 0;
    }

    return api_hijack_enable_set(status);
}

DEFUN(hijack_enable,
      hijack_enable_cmd,
      "hijack switch (on|off)",
      HIJACK_STR
      "switch for enable\n"
      )
{
    return hijack_cmd_enable_set(vty, argv[0]);
}



static int hijack_cmd_get(struct vty *vty, const char *index_str)
{
    int ret = 0;
    uint32_t index = 0;
    hijack_rule_t hijack;
    uint8_t effect = 0;
	
    if (index_str)
    {
        index = atoi(index_str);
        hj_debug("index:%d", index);
    }

    memset(&hijack, 0, sizeof(hijack_rule_t));

    ret = api_hijack_get(index, &hijack, &effect);
    if (ret)
    {
        vty_out(vty, "Hijack dump fail, index(%d) ret(%d)%s", index, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    vty_out(vty, "%-8s %-36s%-36s %-36s%s","index", "host","key", "location", VTY_NEWLINE);
    vty_out(vty, "------------------------------------------------------------%s", VTY_NEWLINE);

    if (HIJACK_RULE_EFFECTIVE == effect)
    {
    	vty_out(vty, "%-8d %-36s%-36s %-36s%s",hijack.index,
                hijack.host, hijack.key, hijack.locate, VTY_NEWLINE);
    }
    else
    {
	vty_out(vty, "Hijack %d does not exist!%s", index, VTY_NEWLINE);
    }
    return CMD_SUCCESS;
}



static int hijack_cmd_get_all(struct vty *vty)
{
    int i, ret = 0;
    uint8_t effect = 0;
    hijack_rule_t hijack;

    memset(&hijack, 0, sizeof(hijack_rule_t));
    vty_out(vty, "%-8s %-36s%-36s %-36s%s","index", "host","key", "location", VTY_NEWLINE);
    for (i = 0; i < HIJACK_RULE_NUM_MAX; i++)
    {
        effect = 0;
        ret = api_hijack_get(i, &hijack, &effect);
        //if (ret)
        //{
            //vty_out(vty, "Hijack dump fail, index(%d) ret(%d)%s", i, ret, VTY_NEWLINE);
            //return CMD_WARNING;
        //}

        if (HIJACK_RULE_EFFECTIVE == effect)
    	{
            vty_out(vty, "%-8d %-36s%-36s %-36s%s",hijack.index,
                    hijack.host, hijack.key, hijack.locate, VTY_NEWLINE);
    	}

    }

    return CMD_SUCCESS;
}

DEFUN(hijack_show,
      hijack_show_cmd,
      "show hijack <0-49>",
      SHOW_STR
      HIJACK_STR
      DEL_STR
      HIJACK_INDEX
      )
{
    return hijack_cmd_get(vty, argv[0]);
}


DEFUN(hijack_show_all,
      hijack_show_all_cmd,
      "show hijack all",
      SHOW_STR
      HIJACK_STR
      DEL_STR
      HIJACK_ALL
      )
{
    return hijack_cmd_get_all(vty);
}


static int hijack_cmd_del(struct vty *vty, const char *index_str)
{
    int ret = 0;
    uint32_t index = 0;

    index = atoi(index_str);
    hj_debug("index:%d", index);

    ret = api_hijack_del(index);
    if (ret)
    {
        vty_out(vty, "hijack del error,index(%d) ret(%d)%s", index, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}


static int hijack_cmd_del_all(struct vty *vty)
{
    int ret = 0;
    int i;

    for (i = 0; i < HIJACK_RULE_NUM_MAX; i++)
    {
        ret = api_hijack_del(i);
        if (ret)
        {
            vty_out(vty, "hijack del error,index(%d) ret(%d)%s", i, ret, VTY_NEWLINE);
            return CMD_WARNING;
        }
    }
    
    return CMD_SUCCESS;
}

DEFUN(hijack_delete,
      hijack_delete_cmd,
      "hijack del <0-49>",
      HIJACK_STR
      DEL_STR
      HIJACK_INDEX
      )
{
    return hijack_cmd_del(vty, argv[0]);
}

DEFUN(hijack_delete_all,
      hijack_delete_all_cmd,
      "hijack del all",
      HIJACK_STR
      DEL_STR
      HIJACK_ALL
      )
{
    return hijack_cmd_del_all(vty);
}


static int hijack_cmd_add(struct vty *vty, const char *index_str, const char *host, const char *key, const char *locate)
{
    int ret = 0;
    uint32_t index = 0;

    hijack_rule_t hijack;

    memset(&hijack, 0 ,sizeof(hijack_rule_t));

    index = atoi(index_str);
    hj_debug("index:%d", index);

    hijack.index = index;
    memcpy(hijack.host, host, strlen(host));
    memcpy(hijack.key, key, strlen(key));
    if (NULL != locate)
    {
        hijack.mode = HIJACK_KEY_MODE;
        memcpy(hijack.locate, locate, strlen(locate));
    }
    else
    {
        hijack.mode = HIJACK_URL_MODE;
    }
    ret = api_hijack_add(&hijack);
    if (ret)
    {
        vty_out(vty, "Hijack add error, index(%d) ret(%d)%s", index, ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}

DEFUN(hijack_set_with_locate,
      hijack_set_with_locate_cmd,
      "hijack add <0-49> HOST KEY LOCATION",
      HIJACK_STR
      ADD_STR
      HIJACK_INDEX
      "Host that is hijacked\n"
      "The value which will replace the original string belongs to url\n"
      "The location which indicates the place that is hijacked\n"
      )
{
    return hijack_cmd_add(vty, argv[0], argv[1], argv[2], argv[3]);
}

DEFUN(hijack_set_without_locate,
      hijack_set_without_locate_cmd,
      "hijack add <0-49> HOST KEY",
      HIJACK_STR
      ADD_STR
      HIJACK_INDEX
      "Host that is hijacked\n"
      "The value which will replace the original url\n"
      )
{
    return hijack_cmd_add(vty, argv[0], argv[1], argv[2], NULL);
}



void hijack_cmd_config_write(struct vty *vty)
{
    int ret = 0;
    uint8_t effect = 0;
    int status = 0;
    int ip_interval = 0, ip_num_interval = 0;
    hijack_rule_t hijack;
    int i;

    if (api_hijack_enable_get(&status))
    {
        return;
    }
    if (1 == status)
    {
        vty_out(vty,"hijack switch on%s", VTY_NEWLINE); 
    }

    for (i = 0; i < HIJACK_RULE_NUM_MAX; i++) 
    {
        effect = 0;
        memset(&hijack, 0 ,sizeof(hijack_rule_t));
        ret = api_hijack_get(i, &hijack, &effect);

        if (HIJACK_RULE_EFFECTIVE == effect)
    	{
            if (HIJACK_KEY_MODE == hijack.mode)
            {
                vty_out(vty, "hijack add %d %s %s %s%s", hijack.index,
                        hijack.host, hijack.key, hijack.locate, VTY_NEWLINE);
            }
            else
            {
                vty_out(vty, "hijack add %d %s %s%s", hijack.index,
                        hijack.host, hijack.key, VTY_NEWLINE);
            }
    	}
    }

    if (api_ip_interval_get(&ip_interval))
    {
        return;
    }
    if (api_ip_num_interval_get(&ip_num_interval))
    {
        return;
    }

    vty_out(vty,"hijack ip interval %d%s", ip_interval, VTY_NEWLINE);
    vty_out(vty,"hijack ip number interval %d%s", ip_num_interval, VTY_NEWLINE);

}

/*
 * hijack module cmdline register and init 
 *
 * */
void cmdline_hijack_init(void)
{
    install_element(CMD_NODE, &ip_num_interval_set_cmd);
    install_element(CMD_NODE, &ip_interval_set_cmd);
    install_element(CMD_NODE, &hijack_set_with_locate_cmd);
    install_element(CMD_NODE, &hijack_set_without_locate_cmd);
    install_element(CMD_NODE, &hijack_show_cmd);
    install_element(CMD_NODE, &hijack_show_all_cmd);
    install_element(CMD_NODE, &hijack_delete_cmd);
    install_element(CMD_NODE, &hijack_delete_all_cmd);
    install_element(CMD_NODE, &show_hijack_interval_cmd);
    install_element(CMD_NODE, &hijack_enable_cmd);

    return;
}
