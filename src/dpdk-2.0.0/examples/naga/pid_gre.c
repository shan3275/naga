#include "naga.h"

berr pid_gtpu(struct pbuf *p, hytag_t *hytag)
{
	struct gre_hdr *grehdr;
	int len = GRE_HEAD_LEN;
	if(check_pbuf_len(p, GRE_HEAD_LEN))
	{
		;//drop packet and incr counter, then return;
		return E_EXCEED;
	}

	PBUF_OFFSET2PTR(struct gre_hdr *, grehdr, p);	

	if(GRE_VERSION(grehdr) != GREV1 
		|| GRE_PRO(grehdr) != GRE_GTP
	)
	{
		//drop and incr;
		return E_FAIL;
	}

	if(GRE_NEXT_HDR_FG(grehdr)
		||GRE_SEQ_NUM_FG(grehdr)
		||GRE_NPDU_FG(grehdr))
	{
		len	+= 4;//
	}
	hytag->teid = ntohl(grehdr->teid);
	
	//skip gre_header
	
		
	
	
}