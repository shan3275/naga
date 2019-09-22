/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-04 10:57
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <string.h>
#include <stdio.h>
#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"
#include "naga_types.h"

#include "bts_cnt.h"
#include "redispush.h"
#include "redispush_cmd.h"

#define REDISPUSH_EXPR        "RAW UDP PUSH\n"
#define REDISPUSH_ADD_EXPR    "Add Operation\n"
#define REDISPUSH_DEL_EXPR    "Del Operation\n"
#define REDISPUSH_TEST_EXPR   "Test Operation\n"
#define REDISPUSH_SERVER_EXPR "RAW UDP Push Server\n"
#define REDISPUSH_PORT_EXPR   "RAW UDP Push Port\n"
#define REDISPUSH_SHOW_EXPR   "Show URL Push Server\n"

DEFUN(redispush_add,
        redispush_add_cmd,
        "redispush add A.B.C.D <0-65535>",
        REDISPUSH_EXPR
        REDISPUSH_ADD_EXPR
        REDISPUSH_SERVER_EXPR
        REDISPUSH_PORT_EXPR)
{
    berr rv;
    rv =  redispush_rule_add((char *)argv[0], (char *)argv[1]);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To add redispush rule %s", VTY_NEWLINE);
        return 0;
    }
    return 0;
}

DEFUN(redispush_del,
        redispush_del_cmd,
        "redispush del",
        REDISPUSH_EXPR
        REDISPUSH_DEL_EXPR)
{
    berr rv;
    rv = redispush_rule_del();
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Del Url rule %s", VTY_NEWLINE);
        return 0;
    }
}

DEFUN(show_redispush,
        show_redispush_cmd,
        "show redispush",
        REDISPUSH_SHOW_EXPR
        REDISPUSH_EXPR)
{
    berr rv;
    uint32_t sip  = 0;
    uint16_t dport = 0;
    rv = redispush_rule_get(&sip, &dport);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Get redispush rule %s", VTY_NEWLINE);
        return 0;
    }
    vty_out(vty, "Raw Udp Push Server: %d.%d.%d.%d:%d%s", (sip >> 24) & 0xff,
                                           (sip >> 16) & 0xff,
                                           (sip >> 8 ) & 0xff,
                                           (sip >> 0 ) & 0xff,
                                           dport,VTY_NEWLINE);
    return 0;
}

DEFUN(redispush_test,
        redispush_test_cmd,
        "redispush test",
        REDISPUSH_EXPR
        REDISPUSH_TEST_EXPR)
{
    berr rv;
    char buff[24]={0};
    rv = redispush_rule_test(buff);
    if(rv != E_SUCCESS)
    {
        if (rv == E_PARAM)
        {
            vty_out(vty, "Sender Server is Null %s", VTY_NEWLINE);
        }
        else
        if (rv == E_FOUND)
        {
            vty_out(vty, "Server is Busy %s", VTY_NEWLINE);
        }
        vty_out(vty, "Failed To Test redispush send cmd %s", VTY_NEWLINE);
        return 0;
    }
    else
    {
        vty_out(vty, "%s %s", buff, VTY_NEWLINE);
    }
}

void redispush_cmd_config_write(struct vty *vty)
{
    berr rv;
    uint32_t sip   = 0;
    uint16_t sport = 0;
    rv = redispush_rule_get(&sip, &sport);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Get redispush rule %s", VTY_NEWLINE);
        return;
    }
    vty_out(vty, "redispush add %d.%d.%d.%d %d%s", (sip >> 24) & 0xff,
                                               (sip >> 16) & 0xff,
                                               (sip >> 8 ) & 0xff,
                                               (sip >> 0 ) & 0xff,
                                               sport,VTY_NEWLINE);
}



void cmdline_redispush_init(void)
{
    install_element(CMD_NODE, &redispush_add_cmd);
    install_element(CMD_NODE, &redispush_del_cmd);
    install_element(CMD_NODE, &redispush_test_cmd);
    install_element(CMD_NODE, &show_redispush_cmd);
    return ;
}
