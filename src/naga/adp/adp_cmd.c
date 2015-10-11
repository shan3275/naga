#include <string.h>

#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"
#include "packet.h"
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
    vty_out(vty, "Interval  : %d%s", interval, VTY_NEWLINE);
    vty_out(vty, "Ad-Total  : %ld%s", adp_count, VTY_NEWLINE);    
    vty_out(vty, "Ad-Success: %ld%s", adp_success, VTY_NEWLINE);  
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



DEFUN(adp_mac_custom_set, 
        adp_mac_custom_set_cmd,
      "adp (dmac|smac) custom", "\n")
{

	int dst_src;
	uint8_t mac[6];
	
	if(argv[0] == NULL)
	{
		vty_out(vty, "PARAM NULL %s", VTY_NEWLINE);
		return E_SUCCESS;
	}
	else
	{
		if(!strcmp(argv[0], "dmac"))
			dst_src = 0;
		else if(!strcmp(argv[0], "smac"))	
			dst_src = 1;
		else
			return E_PARAM;
		
		ads_mac_set(dst_src, 0, mac);	
	}

	
    return 0;
}

DEFUN(adp_mac_special_set, 
        adp_mac_special_set_cmd,
      "adp (dmac|smac) set MAC", "\n")
{

	int dst_src;
	uint8_t mac[6];
	char * mac_str = NULL;
	char *arr = NULL; 
	int i = 0;

	
	if(argv[0] == NULL)
	{
		vty_out(vty, "PARAM NULL %s", VTY_NEWLINE);
		return E_PARAM;
	}
	else
	{
		if(!strcmp(argv[0], "dmac"))
			dst_src = 0;
		else if(!strcmp(argv[0], "smac"))	
			dst_src = 1;
		else
		{
			vty_out(vty, "PARAM smac or dmac %s", VTY_NEWLINE);		
			return E_PARAM;
		}
		if(argv[2]) 
			mac_str = strdup(argv[2]);

		
		while  (NULL != (arr = strsep(&mac_str, ":")))
		{
			if(i == 6)
			{
				vty_out(vty, "PARAM mac format err AA:BB:CC:DD:EE:FF%s", VTY_NEWLINE);				
				return E_PARAM;
			}	
			mac[i++] = atoi(arr);	
		}
		
		ads_mac_set(dst_src, 1, mac);	
	}
	
    return 0;

}



void adp_cmd_config_write(struct vty *vty)
{
    int ret = 0;
	uint8_t mac[6];
    int custom;
	

    ret =  ads_mac_get(0 , &custom, (uint8_t *)mac);
    if(custom == 0)
    {
        vty_out(vty, "adp dmac custom%s", VTY_NEWLINE);  
    }
	else
	{
		vty_out(vty, "adp dmac set %02x:%02x:%02x:%02x:%02x:%02x%s",
						mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
					VTY_NEWLINE); 
	}

	ret =  ads_mac_get(1 , &custom, (uint8_t *)mac);
	if(custom == 0)

	{
		vty_out(vty, "adp smac custom%s", VTY_NEWLINE);
		
	}
	else 
	{
		vty_out(vty, "adp smac set %02x:%02x:%02x:%02x:%02x:%02x%s",
						mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
					VTY_NEWLINE); 	
	}

	
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
	
    install_element(CMD_NODE, &adp_mac_custom_set_cmd);
    install_element(CMD_NODE, &adp_mac_special_set_cmd);	
    return ;
}

