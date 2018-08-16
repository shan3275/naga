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
#include "upush.h"
#include "upush_cmd.h"

#define UPUSH_EXPR        "URL PUSH\n"
#define UPUSH_ADD_EXPR    "Add Operation\n"
#define UPUSH_DEL_EXPR    "Del Operation\n"
#define UPUSH_TEST_EXPR   "Test Operation\n"
#define UPUSH_SERVER_EXPR "URL Push Server\n"
#define UPUSH_SHOW_EXPR   "Show URL Push Server\n"

DEFUN(upush_switch,
        upush_switch_cmd,
        "upush switch (on|off)",
        UPUSH_EXPR
        "switch for upush module!")        
        
{
    int status = 0;
    if (!strcmp("on", argv[0]))
    {   
        status = 1;
    }
    else
    {   
        status = 0;
    }

    return upush_wb_enable_set(status);
}

DEFUN(upush_add,
        upush_add_cmd,
        "upush add SERVER",
        UPUSH_EXPR
        UPUSH_ADD_EXPR
        UPUSH_SERVER_EXPR)
{
    berr rv;
    uint32_t action;
    char * s_str = NULL;
    s_str = strdup(argv[0]);
    rv =  upush_rule_add(s_str);
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To add Url rule %s", VTY_NEWLINE);
        return 0;
    }
    return 0;
}

DEFUN(upush_del,
        upush_del_cmd,
        "upush del",
        UPUSH_EXPR
        UPUSH_DEL_EXPR)
{
    berr rv;
    rv = upush_rule_del();
    if(rv != E_SUCCESS)
    {
        vty_out(vty, "Failed To Del Url rule %s", VTY_NEWLINE);
        return 0;
    }
}

DEFUN(show_upush,
        show_upush_cmd,
        "show upush",
        UPUSH_SHOW_EXPR
        UPUSH_EXPR)
{
    char * s_url = NULL;
    s_url = upush_rule_get();
    if (s_url != NULL)
    {
        vty_out(vty, "Url Push Server: %s%s", s_url, VTY_NEWLINE);
    }
    return 0;
}

DEFUN(upush_test,
        upush_test_cmd,
        "upush test",
        UPUSH_EXPR
        UPUSH_TEST_EXPR)
{
    berr rv;
    rv = upush_rule_test();
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
        vty_out(vty, "Failed To Test upush send cmd %s", VTY_NEWLINE);
        return 0;
    }
}

void upush_cmd_config_write(struct vty *vty)
{
    char * s_url = NULL;
    s_url = upush_rule_get();
    if (s_url != NULL)
    {
        vty_out(vty, "upush add %s%s", s_url, VTY_NEWLINE);
    }
}


void cmdline_upush_init(void)
{
    install_element(CMD_NODE, &upush_add_cmd);
    install_element(CMD_NODE, &upush_del_cmd);
    install_element(CMD_NODE, &upush_test_cmd);
    install_element(CMD_NODE, &show_upush_cmd);
/*for wb business*/
    install_element(CMD_NODE, &upush_switch_cmd);
    return ;
}
