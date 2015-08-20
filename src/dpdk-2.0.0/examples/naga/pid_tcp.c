#ifndef __PID_OUTERL4_H
#define __PID_OUTERL4_H

#include "naga.h"

berr pid_tcp(struct pbuf *p, hytag_t *hytag, int inner_outer)
{
    uint16_t tcphr_len = 0;
	struct tcp_hdr *tcp_hdr;

    if(check_pbuf_len(p, TCP_HLEN))
	{
		;//drop packet and incr counter, then return;
		return E_EXCEED;
	}

	PBUF_OFFSET2PTR(struct tcp_hdr *, tcp_hdr, p);

	if(inner_outer == HEADER_OUTER)
	{
		/* convert src and dest ports to host byte order */
	  	hytag->outer_srcport= ntohs(tcp_hdr->src);
	  	hytag->outer_dstport= ntohs(tcp_hdr->dest);
	}
	else 
	{
		/* convert src and dest ports to host byte order */
	  	hytag->inner_srcport= ntohs(tcp_hdr->src);
	  	hytag->inner_dstport= ntohs(tcp_hdr->dest);		
	}

    tcphr_len = TCP_HDR_LEN(tcp_hdr);
    
    UPDATE_PBUF_OFFSET(p, tcphr_len);

    if(tcp_hdr->src == 80 || tcp_hdr->dest == 80)
    {
         return pid_http(p, hytag);
    }
    else
    {
        //drop;
    }
    return E_SUCCESS;
}


#endif




