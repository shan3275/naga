#ifndef __NAGA_H_
#define __NAGA_H_


typedef struct 
{
	uint32_t outer_srcip4;
	uint32_t outer_dstip4;
	uint32_t inner_srcip4;
	uint32_t inner_dstip4;
#ifdef  _ENABLE_IPV6 
	uint32_t outer_srcip6[4];
	uint32_t outer_dstip6[4];
	uint32_t inner_srcip6[4];
	uint32_t inner_dstip6[4];		
#endif	
	uint32_t teid;
	uint16_t outer_srcport;
	uint16_t outer_dstport;
	uint16_t inner_srcport;
	uint16_t inner_dstport;
	char url[URL_MAX_LEN];
}hytag_t;


struct pbuf {
	//struct pbuf *next; /*always NULL*/
	void 	* ptr;	
	uint16_t  len;  /*this buffer len*/
	//uint16_t  total_len; 
	uint16_t  ptr_offset;/*when The module Pid handle the protocol any*/
	//uint8_t flag;
};	


typedef enum 
{
	E_SUCCESS,
	E_FAIL,
	E_MEMORY,
	E_PARAM,
	E_EXCEED,
	E_OPEN,
	E_FOUND,
	E_EXIST,
	E_BUSY,
	E_MAX,
}berr;


static inline berr check_pbuf_len(struct pbuf *p, int incr_len)
{
	if(p->ptr_offset + incr_len > p->len)
	{
	 	return E_EXCEED;
	}
	else
	{
		return E_SUCCESS;
	}
}

#define UPDATE_PBUF_OFFSET(_pbuf, _off) (_pbuf->ptr_offset+=_off)

#define PBUF_OFFSET2PTR(_type, _ptr, _p) (_ptr = (_type)(_p->ptr_offset+_p->ptr))

#define BYTE_ORDER  LITTLE_ENDIAN

#define HEADER_OUTER  0
#define HEADER_INNER  1

#endif

