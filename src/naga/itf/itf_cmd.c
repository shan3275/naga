#include <string.h>

#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#include "naga_types.h"
#include "itf.h"
#include "itf_cmd.h"
#include "itf_stat.h"

DEFUN(itf_rxtx, 
      itf_rxtx_cmd,
      "interface enable rx eth IFNAME", "interface setting\nenable or disable\nrx or tx\n")
{
    if(argv[0] != NULL)
    {
        char * ifname = strdup(argv[0]);
        berr rv=  libpcap_rx_loop_setup(ifname);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to open %s rx%s", ifname, VTY_NEWLINE);
        else
            vty_out(vty, "Failed to open %s rx%s", ifname, VTY_NEWLINE);
        free(ifname);
        return 0;
    }
    return 0;
}

static int itf_cmd_show_status(struct vty *vty)
{
    int rv;
    itf_stat stat;
    stat.direction = ITF_TX;
    rv = itf_stat_get( &stat);
    if (rv)
    {
        vty_out(vty, "get tx status fail rv(%d)%s", rv, VTY_NEWLINE);
    }
    else
    {
        vty_out(vty, "TX :%s%s", stat.enable == ITF_ENABLE ?"Enable":"Disable", VTY_NEWLINE);
    }

    stat.direction = ITF_RX;
    rv = itf_stat_get( &stat);
    if (rv)
    {
        vty_out(vty, "get rx status fail rv(%d)%s", rv, VTY_NEWLINE);
    }
    else
    {
        vty_out(vty, "RX :%s%s", stat.enable == ITF_ENABLE ?"Enable":"Disable", VTY_NEWLINE);
    }

    return CMD_SUCCESS;
}

DEFUN(itf_show_stat, 
      itf_show_stat_cmd,
      "show itf status",
      SHOW_STR
      INTERFACE_STR
      "Status information\n" 
      )
{
    return itf_cmd_show_status(vty);
}


static int itf_cmd_set(struct vty *vty, const char *dir_str, const char *en_str)
{
    int ret = 0;
    itf_stat stat;
    if ('t' == dir_str[0])
    {
        stat.direction = ITF_TX;
    }
    else
    if ('r' == dir_str[0])
    {
        stat.direction = ITF_RX;
    }
    else
    {
        vty_out(vty, "parameter err %s %s", dir_str, VTY_NEWLINE);
        return CMD_WARNING;
    }

    if ('e' == en_str[0])
    {
        stat.enable = ITF_ENABLE;
    }
    else
    if ('d' == en_str[0])
    {
        stat.enable = ITF_DISABLE;
    }
    else
    {
        vty_out(vty, "parameter err %s %s", en_str, VTY_NEWLINE);
        return CMD_WARNING;
    }

    ret = itf_stat_set(&stat);
    if (ret)
    {
        vty_out(vty, "itf stat set fail ret(%d)%s", ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}
DEFUN(itf_set,
      itf_set_cmd,
      "itf (rx|tx) (enable|disable)",
      INTERFACE_STR
      "Rx or Tx direction\n"
      "En or Disable operation\n" 
      )
{
    return itf_cmd_set(vty, argv[0], argv[1]);
}

void itf_cmd_config_write(struct vty *vty)
{
    int rv;
    itf_stat stat;
    stat.direction = ITF_TX;
    rv = itf_stat_set( &stat);
    if ( E_SUCCESS == rv)
    {
        vty_out(vty, "itf tx %s%s", stat.enable == ITF_ENABLE ?"enable":"disable", VTY_NEWLINE);
    }

    stat.direction = ITF_RX;
    rv = itf_stat_set( &stat);
    if ( E_SUCCESS == rv)
    {
        vty_out(vty, "itf rx %s%s", stat.enable == ITF_ENABLE ?"enable":"disable", VTY_NEWLINE);
    }
}






/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_itf_init(void)
{
    install_element(CMD_NODE, &itf_rxtx_cmd);
    install_element(CMD_NODE, &itf_show_stat_cmd);
    install_element(CMD_NODE, &itf_set_cmd);

    return ;
}


