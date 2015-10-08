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

DEFUN(adp_interval_get, 
      adp_interval_get_cmd,
      "adp interval get", "Adp module\ninterval\nget interval http-get packet <1-100000> url will\n")
{
    int interval;
    uint64_t adp_count;
    uint64_t adp_success;
    adp_get_interval(&interval, &adp_count, &adp_success);  
    vty_out(vty, "Interval  : %d\n", interval);
    vty_out(vty, "Ad-Total  : %ld\n", adp_count);    
    vty_out(vty, "Ad-Success: %ld\n", adp_success);  
    return 0;
}



DEFUN(adp_switch_set_on, 
      adp_switch_set_on_cmd,
      "adp switch on", "\n")
{
    adp_switch_set(1);
    return 0;
}

DEFUN(adp_switch_set_off, 
      adp_switch_set_off_cmd,
      "adp switch off", "\n")
{

    adp_switch_set(0);
    return 0;
}





/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_adp_init(void)
{
    
    install_element(CMD_NODE, &adp_interval_cmd);
    install_element(CMD_NODE, &adp_interval_get_cmd);
    install_element(CMD_NODE, &adp_switch_set_on_cmd); 
    install_element(CMD_NODE, &adp_switch_set_off_cmd);     
    return ;
}

