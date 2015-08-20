#include "naga.h"


berr pid_udp(struct pbuf *p, hytag_t *hytag, int inner_outer)
{

	struct udp_hdr *udphdr;
	if(check_pbuf_len(p, UDP_HLEN))
	{
		;//drop packet and incr counter, then return;
		return E_EXCEED;
	}

	PBUF_OFFSET2PTR(struct udp_hdr *, udphdr, p);

	if(inner_outer == HEADER_OUTER)
	{
		/* convert src and dest ports to host byte order */
	  	hytag->outer_srcport= ntohs(udphdr->src);
	  	hytag->outer_dstport= ntohs(udphdr->dest);
	}
	else 
	{
		/* convert src and dest ports to host byte order */
	  	hytag->inner_srcport= ntohs(udphdr->src);
	  	hytag->inner_dstport= ntohs(udphdr->dest);		
	}

	if( hytag->outer_srcport == UDP_PORT_GTP_U
		           && hytag->outer_dstport == UDP_PORT_GTP_U)//gtp-u
	{
		if (likely(inner_outer == HEADER_OUTER))
		{
			UPDATE_PBUF_OFFSET(p, UDP_HLEN);
			return pid_gtpu(p, hytag);
		}
		else
		{
			//incr cnt and drop
		}
	}
	else 
	{
		//drop and incr 
	}
	return E_SUCCESS;
									
}


