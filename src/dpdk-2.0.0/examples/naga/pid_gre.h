#ifndef __GRE_H_
#define __GRE_H_

struct gre_hdr
{
#if BYTE_ORDER == LITTLE_ENDIAN
	uint8_t protype:1;
	uint8_t reserv:1;
	uint8_t next_header:1;
	uint8_t sequence_num:1;
	uint8_t npdu:1;	
	uint8_t vesion:3;
#else
	uint8_t vesion:3;
	uint8_t protype:1;
	uint8_t reserv:1;
	uint8_t next_header:1;
	uint8_t sequence_num:1;
	uint8_t npdu:1;
#endif
	uint8_t message_type;
	uint16_t length;
	uint32_t teid;
};



#define GRE_VERSION(grehdr) (grehdr->vesion)
#define GRE_PRO(grehdr) 	(grehdr->protype)
#define GRE_VERSION(grehdr) (grehdr->vesion)
#define GRE_NEXT_HDR_FG(grehdr) (grehdr->next_header)
#define GRE_SEQ_NUM_FG(grehdr)  (grehdr->sequence_num)
#define GRE_NPDU_FG(grehdr)     		(grehdr->npdu)

berr pid_gre(struct pbuf *p, hytag_t *hytag);

#define GRE_HEAD_LEN  8


#endif
