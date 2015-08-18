#ifndef __PID_ETHERNET_H
#define __PID_ETHERNET_H

#include <sys/types.h>
#include "naga.h"

#define ETHARP_HWADDR_LEN     6

struct eth_addr {
 	uint8_t addr[ETHARP_HWADDR_LEN];
};


struct eth_hdr {
	struct eth_addr dest;
   	struct eth_addr src;
  	uint16_t ethertype;
} ;



berr pid_ethernet(struct pbuf *p, hytag_t *hytag);




#endif
