#include "naga.h"


berr pid_outerip4(struct pbuf *p,  hytag_t * hytag)
{
	struct ip4_hdr *iphdr;
	uint16_t  iphdr_hlen, iphdr_len;

	/* identify the IP header */
  	iphdr = (struct ip4_hdr *)p->payload;

	
	if (IPH_V(iphdr) != 4) 
	{
		//incr err
		return ;
	}
	
  	/* obtain IP header length in number of 32-bit words */
  	iphdr_hlen = IPH_HL(iphdr);
	/* calculate IP header length in bytes */
  	iphdr_hlen *= 4;
	/* obtain ip length in bytes */
  	iphdr_len = ntohs(IPH_LEN(iphdr));

	hytag->outer_dstip4 = ntohs(iphdr->dest);
	hytag->outer_srcip4 = ntohs(iphdr->src);

	return pid_outerL4(p, hytag, IPH_PROTO(iphdr));
		
}

