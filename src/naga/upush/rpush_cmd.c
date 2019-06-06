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
    uint32_t sip  = 0;
    uint8_t  sip_eanble = 0;
    uint32_t dip   = 0;
    uint16_t dport = 0;
    rv = rpush_rule_get(&sip, &sip_eanble, &dip, &dport);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Get rpush rule %s", VTY_NEWLINE);
        return 0;
    }
    vty_out(vty, "Raw Udp Push Source: %d.%d.%d.%d(%s)%s", (sip >> 24) & 0xff,
                                           (sip >> 16) & 0xff,
                                           (sip >> 8 ) & 0xff,
                                           (sip >> 0 ) & 0xff,
                                           sip_eanble?"enable":"disable",VTY_NEWLINE);
    vty_out(vty, "Raw Udp Push Server: %d.%d.%d.%d:%d%s", (dip >> 24) & 0xff,
                                           (dip >> 16) & 0xff,
                                           (dip >> 8 ) & 0xff,
                                           (dip >> 0 ) & 0xff,
                                           dport,VTY_NEWLINE);
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
    uint32_t sip  = 0;
    uint8_t  sip_eanble = 0;
    uint32_t dip   = 0;
    uint16_t dport = 0;
    rv = rpush_rule_get(&sip, &sip_eanble, &dip, &dport);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Get rpush rule %s", VTY_NEWLINE);
        return;
    }
    vty_out(vty, "rpush sip %s%s", sip_eanble?"enable":"disable",VTY_NEWLINE);
    vty_out(vty, "rpush sip set %d.%d.%d.%d%s",    (sip >> 24) & 0xff,
                                               (sip >> 16) & 0xff,
                                               (sip >> 8 ) & 0xff,
                                               (sip >> 0 ) & 0xff,
                                                VTY_NEWLINE);
    vty_out(vty, "rpush add %d.%d.%d.%d %d%s", (dip >> 24) & 0xff,
                                               (dip >> 16) & 0xff,
                                               (dip >> 8 ) & 0xff,
                                               (dip >> 0 ) & 0xff,
                                               dport,VTY_NEWLINE);
}

DEFUN(rpush_sip_enable,
      rpush_sip_enable_cmd,
      "rpush sip (enable|disable)",
      RPUSH_EXPR
      "source ip\n"
      "En or Disable operation\n" 
      )
{
    return rpush_sip_enable_set(argv[0]);
}

DEFUN(rpush_sip_add,
      rpush_sip_add_cmd,
      "rpush sip set A.B.C.D",
      RPUSH_EXPR
      "source ip\n"
      "set\n"
      RPUSH_SERVER_EXPR)
{
    berr rv;
    rv =  rpush_rule_sip_add(argv[0]);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To add rpush sip rule %s", VTY_NEWLINE);
        return 0;
    }
    return 0;
}

void cmdline_rpush_init(void)
{
    install_element(CMD_NODE, &rpush_add_cmd);
    install_element(CMD_NODE, &rpush_del_cmd);
    install_element(CMD_NODE, &rpush_test_cmd);
    install_element(CMD_NODE, &show_rpush_cmd);
    install_element(CMD_NODE, &rpush_sip_enable_cmd);
    install_element(CMD_NODE, &rpush_sip_add_cmd);

    return ;
}
