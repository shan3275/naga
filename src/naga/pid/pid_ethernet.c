#include "pid.h"



berr pid_ethernet(struct pbuf *p, hytag_t *hytag)
{
    struct eth_pppoe_hdr *eth_pppoe_heder = NULL;
	struct eth_vlan_hdr *eth_v_header = NULL;
    struct eth_hdr * eth_header = NULL;
	uint16_t type = 0;
	uint16_t len = 0;

	
	if(check_pbuf_len(p, SIZEOF_ETH_HDR))
	{
		pid_incr_count(HEADER_UNEXPECT);
		BRET(E_EXCEED);
	}
	
	PBUF_CUR_FORMAT(struct eth_hdr *, eth_header, p);

    hytag->l2_offset = p->ptr_offset;
	type = eth_header->ethertype;
	len += SIZEOF_ETH_HDR; 
	DEBUG_PRINTF("DstMac=%02x:%02x:%02x:%02x:%02x:%02x\n",
                     eth_header->dest.addr[0], eth_header->dest.addr[1], eth_header->dest.addr[2],
                     eth_header->dest.addr[3], eth_header->dest.addr[4], eth_header->dest.addr[5]);
    DEBUG_PRINTF("SrcMac=%02x:%02x:%02x:%02x:%02x:%02x\n", 
                      eth_header->src.addr[0], eth_header->src.addr[1], eth_header->src.addr[2],
                     eth_header->src.addr[3], eth_header->src.addr[4], eth_header->src.addr[5]);
    DEBUG_PRINTF("Ethertype=%04x\n", ntohs(type)); 

    
	/*skip vlan*/
	if(type == htons(ETHERTYPE_VLAN))
	{
		pid_incr_count(VLAN_PKTS);
		do{
			eth_v_header = (struct eth_vlan_hdr*)PBUF_PTR(p, len);	
		}while(eth_v_header->v_type== htons(ETHERTYPE_VLAN) && (len += 4));
		type = eth_v_header->v_type;
        len += 4;
//        printf("vlan len = %d\n", len);
	}

    /*  skip  PPPoE */
    if(type == htons(ETHERTYPE_PPPOE))
    {
        pid_incr_count(PPPOE_PKTS);
        eth_pppoe_heder = (struct eth_pppoe_hdr *)PBUF_PTR(p, len) ;
        if(eth_pppoe_heder->proto == htons(PPP_PROTO_IP4))
        {
            type = htons(ETHERTYPE_IP4);
        }
        len += 8;
//        printf("pppoe len = %d\n", len);
//        printf("pppoe proto = %x\n", ntohs(type));
    }

    UPDATE_PBUF_OFFSET(p, len);	
    hytag->l3_offset = p->ptr_offset;
    
	switch( ntohs(type) )
	{
		case ETHERTYPE_IP4:
			pid_incr_count(OUTERL3_IPV4);
            return pid_outerip4(p, hytag);
		case ETHERTYPE_IP6:
			pid_incr_count(OUTERL3_IPV6);
            break;
		default:
			pid_incr_count(OUTERL3_NOIP);
            break;
	}
	return E_SUCCESS;
}




