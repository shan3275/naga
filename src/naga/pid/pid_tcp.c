#ifndef __PID_OUTERL4_H
#define __PID_OUTERL4_H

#include "pid.h"

berr pid_tcp(struct pbuf *p, hytag_t *hytag, int inner_outer)
{
    uint16_t tcphr_len = 0;
	struct tcp_hdr *tcp_hdr;

    if(check_pbuf_len(p, TCP_HLEN))
	{
        if(inner_outer == HEADER_OUTER)
            pid_incr_count(OUTERL4_HD);//drop packet and incr counter, then return;
        else
            pid_incr_count(INNERL4_HD);
		return E_EXCEED;
	}

	PBUF_CUR_FORMAT(struct tcp_hdr *, tcp_hdr, p);

	if(inner_outer == HEADER_OUTER)
	{
		/* convert src and dest ports to host byte order */
	  	hytag->outer_srcport= ntohs(tcp_hdr->src);
	  	hytag->outer_dstport= ntohs(tcp_hdr->dest);
        pid_incr_count(OUTERL4_TCP);
	}
	else 
	{
		/* convert src and dest ports to host byte order */
	  	hytag->inner_srcport= ntohs(tcp_hdr->src);
	  	hytag->inner_dstport= ntohs(tcp_hdr->dest);
        pid_incr_count(INNERL4_TCP);
	}

    tcphr_len = TCP_HDR_LEN(tcp_hdr);
    
    UPDATE_PBUF_OFFSET(p, tcphr_len);

    switch(ntohs(tcp_hdr->dst_port))
    {
        case 80:
        case 8080:
             pid_incr_count(APP_HTTP);
             return pid_http(p, hytag);        
        default:
             pid_incr_count(APP_OTHER);
			 break;
    }
    
    return E_SUCCESS;
}


#endif




