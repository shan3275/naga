#include "naga.h"


#define SIZEOF_ETH_HDR 	(14)
#define ETHERTYPE_IP 		0x0800
#define ETHERTYPE_VLAN		0x8100


int pid_ethernet(struct pbuf *p, hytag_t *hytag)
{
	char *buf = p->ptr;
	struct eth_vlan_hdr *eth_v_header = NULL;
	uint16_t type;
	int len = 0;

	
	if(unlikely(p->total_len < sizeof(struct eth_hdr))
	{
		//Packet undersize cnt incr;
		return -1;
	}
	
	struct eth_hdr *eth_header = (struct eth_hdr *)buf;
	type = eth_header->ethertype;
	len += SIZEOF_ETH_HDR;
	
		
	/*skip vlan*/
	if(type == htons(ETHERTYPE_VLAN))
	{
		len-=2;
		do{
			eth_v_header = (struct eth_vlan_hdr*)(buf+len);	
		}while(eth_v_header == htons(ETH_VLAN));
		len -=4;
		type = eth_v_header->v_type;
	}

	

	switch( ntohs(type) )
	{
		case ETHERTYPE_IP:
			break;
		case 	
	}
	
}




