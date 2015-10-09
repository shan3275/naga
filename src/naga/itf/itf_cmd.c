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

DEFUN(itf_rxtx, 
      itf_rxtx_cmd,
      "interface {enable|disable} rx eth IFNAME", "interface setting\nenable or disable\nrx or tx\n")
{
    if(argv[0] != NULL)
    {
        char * ifname = strdup(argv[0]);
        berr rv=  libpcap_rx_loop_setup(ifname);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to open %s rx", ifname);
        else
            vty_out(vty, "Failed to open %s rx", ifname);
        free(ifname);
        return 0;
    }
    return 0;
}








/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_itf_init(void)
{
    install_element(CMD_NODE, &itf_rxtx_cmd);

    return ;
}


