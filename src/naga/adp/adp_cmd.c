#include <string.h>

#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#include "naga_types.h"
#include "nag_adp.h"
#include "adp_cmd.h"

DEFUN(adp_interval, 
      adp_interval_cmd,
      "adp interval <1-100000>", "Adp module\ninterval\nper http-get packet <1-100000> url will\n")
{
    int interval = strtoul(argv[0], NULL, 0);
    return adp_set_interval(interval);
}


/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_adp_init(void)
{

    install_element(CMD_NODE, &adp_interval_cmd);
    return ;
}

