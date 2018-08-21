#ifndef __PID_ETHERNET_H
#define __PID_ETHERNET_H

#include <sys/types.h>
#include "pid.h"

#define ETHARP_HWADDR_LEN     6

struct eth_addr {
 	uint8_t addr[ETHARP_HWADDR_LEN];
};


struct eth_hdr {
	struct eth_addr dest;
   	struct eth_addr src;
  	uint16_t ethertype;
} ;


struct eth_vlan_hdr 
{
    uint16_t v_val;	
 	uint16_t v_type;
};



struct eth_pppoe_hdr 
{
    uint8_t  ver_type;
    uint8_t  data;
    uint16_t  id;
    uint16_t  plen;
    uint16_t  proto;
};




berr pid_ethernet(struct pbuf *p, hytag_t *hytag);




#endif
