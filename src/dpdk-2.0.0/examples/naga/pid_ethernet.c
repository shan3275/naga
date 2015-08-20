

#include "naga.h"


#define SIZEOF_ETH_HDR 	(14)
#define ETHERTYPE_IP4 		0x0800
#define ETHERTYPE_VLAN		0x8100


berr pid_ethernet(struct pbuf *p, hytag_t *hytag)
{

	struct eth_vlan_hdr *eth_v_header = NULL;
    struct eth_hdr * eth_header;
	uint16_t type;
	int len = 0;

	
	if(check_pbuf_len(p, SIZEOF_ETH_HDR))
	{
		;//drop packet and incr counter, then return;
		return E_EXCEED;
	}
	
	PBUF_OFFSET2PTR(struct eth_hdr *, eth_header, p);

	type = eth_header->ethertype;
	len += SIZEOF_ETH_HDR; 
		
	/*skip vlan*/
	if(type == htons(ETHERTYPE_VLAN))
	{
		len-=2;
		do{
            //PBUF_OFFSET2PTR(struct eth_hdr *, eth_v_header, p);
			eth_v_header = (struct eth_vlan_hdr*)PBUF_PTR(p, len);	
            len += 4;
		}while(eth_v_header->v_type== htons(ETHERTYPE_VLAN));
		len -=4;
		type = eth_v_header->v_type;
	}

	

	switch( ntohs(type) )
	{
		case ETHERTYPE_IP4:
            return pid_outerip4(p, hytag);
		default:
            break;
	}
	return E_SUCCESS;
}




