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
#include "rpush_cmd.h"

#define RPUSH_EXPR        "RAW UDP PUSH\n"
#define RPUSH_ADD_EXPR    "Add Operation\n"
#define RPUSH_DEL_EXPR    "Del Operation\n"
#define RPUSH_TEST_EXPR   "Test Operation\n"
#define RPUSH_SERVER_EXPR "RAW UDP Push Server\n"
#define RPUSH_PORT_EXPR   "RAW UDP Push Port\n"
#define RPUSH_SHOW_EXPR   "Show URL Push Server\n"

DEFUN(rpush_add,
        rpush_add_cmd,
        "rpush add A.B.C.D <0-65535>",
        RPUSH_EXPR
        RPUSH_ADD_EXPR
        RPUSH_SERVER_EXPR
        RPUSH_PORT_EXPR)
{
    berr rv;
    rv =  rpush_rule_add(argv[0], argv[1]);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To add rpush rule %s", VTY_NEWLINE);
        return 0;
    }
    return 0;
}

DEFUN(rpush_del,
        rpush_del_cmd,
        "rpush del",
        RPUSH_EXPR
        RPUSH_DEL_EXPR)
{
    berr rv;
    rv = rpush_rule_del();
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Del Url rule %s", VTY_NEWLINE);
        return 0;
    }
}

DEFUN(show_rpush,
        show_rpush_cmd,
        "show rpush",
        RPUSH_SHOW_EXPR
        RPUSH_EXPR)
{
    berr rv;
    uint32_t ip   = 0;
    uint16_t port = 0;
    rv = rpush_rule_get(&ip, &port);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Get rpush rule %s", VTY_NEWLINE);
        return 0;
    }
    vty_out(vty, "Raw Udp Push Server: %d.%d.%d.%d:%d%s", (ip >> 24) & 0xff,
                                           (ip >> 16) & 0xff,
                                           (ip >> 8 ) & 0xff,
                                           (ip >> 0 ) & 0xff,
                                           port,VTY_NEWLINE);
    return 0;
}

DEFUN(rpush_test,
        rpush_test_cmd,
        "rpush test",
        RPUSH_EXPR
        RPUSH_TEST_EXPR)
{
    berr rv;
    rv = rpush_rule_test();
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
        vty_out(vty, "Failed To Test rpush send cmd %s", VTY_NEWLINE);
        return 0;
    }
}

void rpush_cmd_config_write(struct vty *vty)
{
    berr rv;
    uint32_t ip   = 0;
    uint16_t port = 0;
    rv = rpush_rule_get(&ip, &port);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Get rpush rule %s", VTY_NEWLINE);
        return ;
    }
    vty_out(vty, "rpush add %d.%d.%d.%d %d%s", (ip >> 24) & 0xff,
                                               (ip >> 16) & 0xff,
                                               (ip >> 8 ) & 0xff,
                                               (ip >> 0 ) & 0xff,
                                               port,VTY_NEWLINE);
}


void cmdline_rpush_init(void)
{
    install_element(CMD_NODE, &rpush_add_cmd);
    install_element(CMD_NODE, &rpush_del_cmd);
    install_element(CMD_NODE, &rpush_test_cmd);
    install_element(CMD_NODE, &show_rpush_cmd);
    return ;
}
