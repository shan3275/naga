#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <string.h>
#include <sys/queue.h>
#include <stdarg.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>

#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#include <rte_debug.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_cycles.h>
#include <rte_mbuf.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include <rte_lpm.h>
#include <rte_lpm6.h>
#include "l3fwd.h"

#include "naga_types.h"
#include "core_cmd.h"

extern struct l3fwd_core_statistics core_statistics[RTE_MAX_LCORE];

static int core_cmd_show_status(struct vty *vty)
{
    time_t t;
    int i;

    time(&t);
    vty_out(vty, "Statistics of date and time: %s%s", ctime(&t), VTY_NEWLINE);

    for(i = 0; i < RTE_MAX_LCORE; i++)
    {
        if (core_statistics[i].rx)
        {
            vty_out(vty, "Core %02d    RX: %lld%s", i, (ULL) core_statistics[i].rx, VTY_NEWLINE);
        }

        if (core_statistics[i].fail)
        {
            vty_out(vty, "Core %02d  Fail: %lld%s", i, (ULL) core_statistics[i].fail, VTY_NEWLINE);
        }

    }
    return CMD_SUCCESS;
}

DEFUN(core_show_stat, 
      core_show_stat_cmd,
      "show core stat",
      SHOW_STR
      "DPDP CORE"
      "Stastics information\n" 
      )
{
    return core_cmd_show_status(vty);
}

static int core_cmd_clear_stat(void) 
{
    int i;
    for(i = 0; i < RTE_MAX_LCORE; i++)
    {
        core_statistics[i].rx = 0;
        core_statistics[i].fail = 0;
    }

    return CMD_SUCCESS;
}

DEFUN(core_clear_stat,
        core_clear_stat_cmd,
        "clear core stat",
        CLEAR_STR
        "DPDP CORE"
        "Stastics information\n"
        )
{
    return core_cmd_clear_stat();
}

/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_core_init(void)
{
    install_element(CMD_NODE, &core_show_stat_cmd);
    install_element(CMD_NODE, &core_clear_stat_cmd);

    return ;
}


