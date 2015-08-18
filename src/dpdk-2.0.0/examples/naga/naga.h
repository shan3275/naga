#ifndef __NAGA_H_
#define __NAGA_H_

typedef struct 
{
	uint32_t outer_srcip;
	uint32_t outer_dstip;
	uint32_t inner_srcip;
	uint32_t inner_dstip;
	uint32_t teid;
	uint16_t inner_srcport;
	uint16_t inner_dstport;
	char url[URL_MAX_LEN];
}hytag_t;


struct pbuf {
	//struct pbuf *next; /*always NULL*/
	void * ptr;	
	uint16_t  len;  /*this buffer len*/
	uint16_t  total_len;
	uint16_t pid_offset;/*when The module Pid handle the protocol any*/
	uint8_t flag;
};	





#define BYTE_ORDER  LITTLE_ENDIAN


#endif

