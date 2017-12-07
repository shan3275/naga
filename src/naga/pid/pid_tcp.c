#ifndef __PID_OUTERL4_H
#define __PID_OUTERL4_H

#include "pid.h"

#define HTTP_PORT_80 	80
#define HTTP_PORT_8080 	80


berr pid_tcp(struct pbuf *p, hytag_t *hytag, int inner_outer)
{
    uint16_t tcphr_len = 0;
	struct tcp_hdr_s *tcp_hdr;

    if(check_pbuf_len(p, TCP_HLEN))
	{
        if(inner_outer == HEADER_OUTER)
            pid_incr_count(OUTERL4_HD);//drop packet and incr counter, then return;
        else
            pid_incr_count(INNERL4_HD);
		BRET(E_EXCEED);
	}

	PBUF_CUR_FORMAT(struct tcp_hdr_s *, tcp_hdr, p);

	if(inner_outer == HEADER_OUTER)
	{
		/* convert src and dest ports to host byte order */
	  	hytag->outer_srcport= ntohs(tcp_hdr->src);
	  	hytag->outer_dstport= ntohs(tcp_hdr->dest);
        pid_incr_count(OUTERL4_TCP);
        hytag->outer_seq = ntohl(tcp_hdr->seq);
        hytag->outer_ack = ntohl(tcp_hdr->ack_seq);
#ifdef HTTP_BLOCK
        hytag->http_block.sport = hytag->outer_srcport;
        hytag->http_block.dport = hytag->outer_dstport;
        hytag->http_block.seq     = hytag->outer_seq;
        hytag->http_block.ack     = hytag->outer_ack;
#endif

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
    hytag->l5_offset = p->ptr_offset;
    hytag->l5_len    = hytag->total_len - ( hytag->l5_offset - hytag->l3_offset);
    DEBUG_PRINTF("p->len:%d\n", p->len); 
    DEBUG_PRINTF("hytag->l5_offset = p->ptr_offset:%d\n", hytag->l5_offset); 
    DEBUG_PRINTF("hytag->l5_len :%d\n", hytag->l5_len); 

	
	uint16_t srcport = ntohs(tcp_hdr->src);
	uint16_t dstport = ntohs(tcp_hdr->dest);
	
	if( dstport == 80 || dstport == 8080 )
	{
	     pid_incr_count(APP_HTTP);
         pid_add_count(APP_HTTP_BYTES,hytag->pbuf.len);
		 return pid_http_up(p, hytag);
	}
    else if(srcport == 80 || srcport == 8080 )
    {
	     pid_incr_count(APP_HTTP);
         pid_add_count(APP_HTTP_BYTES,hytag->pbuf.len);
		 return pid_http_down(p, hytag);
    }
	else
	{
	     pid_incr_count(APP_OTHER);
         pid_add_count(APP_OTHER_BYTES,hytag->pbuf.len);
	}
    return E_SUCCESS;
}


#endif




