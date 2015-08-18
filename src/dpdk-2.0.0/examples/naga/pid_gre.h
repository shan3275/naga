#ifndef __GRE_H_
#define __GRE_H_

struct gre_head_t
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



berr pid_gre(struct pbuf *p, hytag_t *hytag);



#endif
