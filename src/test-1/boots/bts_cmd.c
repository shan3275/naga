/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/naga/boots\bts_cmd.c
#       @date         :2015-09-03 13:32
#       @algorithm    :
=============================================================================*/
#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#include "bts_cnt.h"
#include "bts_debug.h"

#define STAT_STR                    "Statistics Operation\n"
#define ALL_STR                     "All list\n"
#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  vty_out(vty, "func=%s line=%d:" fmt "%s", __FUNCTION__, __LINE__, ##args, VTY_NEWLINE)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

static int bts_cmd_clear_all(struct vty *vty)
{
    uint32_t total = 0;
    if (cnt_clear(0, CNT_MAX, &total))
    {
        return CMD_WARNING;
    }
    debug("total: %d", total);
    return CMD_SUCCESS;
}

DEFUN(bts_clear_all,
      bts_clear_all_cmd,
      "clear stat all",
      CLEAR_STR
      STAT_STR
      ALL_STR
      )
{
    return bts_cmd_clear_all(vty);
}

static int bts_cmd_show_all(struct vty *vty)
{

    int i;
    uint32_t total = 0;
    cnt_t cnt_array[CNT_MAX];
    if (cnt_get(0, CNT_MAX, cnt_array, &total))
    {
        debug("total: %d", total);
        return CMD_WARNING;
    }
    debug("total: %d", total);
    for ( i = 0; i < CNT_MAX; i++)
    {
        //if (cnt_array[i].val.cnt > 0)  
        {
            vty_out(vty, "%-40s: %lld%s", cnt_array[i].name, (ULL)cnt_array[i].val.cnt, VTY_NEWLINE);
        }
    }
    return CMD_SUCCESS;
}

DEFUN(bts_show_all,
      bts_show_all_cmd,
      "show stat all",
      SHOW_STR
      STAT_STR
      ALL_STR
      )
{
    return bts_cmd_show_all(vty);
}

/*
 * bts_cnt  module cmdline register and init 
 *
 * */
void cmdline_bts_init(void)
{
    install_element(CMD_NODE, &bts_show_all_cmd);
    install_element(CMD_NODE, &bts_clear_all_cmd);

    return ;
}
