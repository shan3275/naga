#include "naga.h"


berr pid_outerip4(struct pbuf *p,  hytag_t * hytag)
{
	struct ip4_hdr *iphdr;
	uint16_t  iphdr_hlen;//, iphdr_len;


	if(check_pbuf_len(p, IP_HD_LEN))
	{
		;//drop packet and incr counter, then return;
		return E_EXCEED;
	}
	
	if(check_pbuf_len(p, IP_HD_LEN))
	{
		;//drop packet and incr counter, then return;
		return E_EXCEED;
	}

    
	PBUF_OFFSET2PTR(struct ip4_hdr *, iphdr, p);
	if (IPH_V(iphdr) != 4) 
	{
		//incr err
		return E_COMPARE;
	}
	
  	/* obtain IP header length in number of 32-bit words */
  	iphdr_hlen = IPH_HL(iphdr);
	/* calculate IP header length in bytes */
  	iphdr_hlen *= 4;
	/* obtain ip length in bytes */
  	//iphdr_len = ntohs(IPH_LEN(iphdr));

	hytag->outer_dstip4 = ntohs(iphdr->dest);
	hytag->outer_srcip4 = ntohs(iphdr->src);

	UPDATE_PBUF_OFFSET(p, iphdr_hlen);	
	return pid_L4(p, hytag, IPH_PROTO(iphdr), HEADER_OUTER);
		
}

