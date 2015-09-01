#ifndef __PID_H_
#define __PID_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <rte_branch_prediction.h>
#include "boots.h"

typedef enum
{
	URL_IN_NULL,
	URL_IN_GTP,
} APP_TYPE_E;


#define URL_MAX_LEN  512 //URL MAX LEN
typedef struct 
{
    /*OT L3*/
	uint32_t outer_srcip4;
	uint32_t outer_dstip4;
    /*OT L4 Protoco;*/
    uint8_t outer_protocol;
    /*OT L4*/
	uint16_t outer_srcport;
	uint16_t outer_dstport;

    uint32_t teid;
    /*IN L3*/
	uint32_t inner_srcip4;
	uint32_t inner_dstip4;
    /*IN L4 Protoco;*/
    uint8_t inner_protocol;    
    /*IN L4*/
	uint16_t inner_srcport;
	uint16_t inner_dstport;


#ifdef  _ENABLE_IPV6 
	uint32_t outer_srcip6[4];
	uint32_t outer_dstip6[4];
	uint32_t inner_srcip6[4];
	uint32_t inner_dstip6[4];		
#endif	

    uint16_t app_type;
    uint16_t url_len;
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

#define PBUF_CUR_FORMAT(_type, _ptr, _p) (_ptr = (_type)(_p->ptr_offset+(char *)_p->ptr))
#define PBUF_OFF_FORMAT(_type, _ptr, _p,_len) (_ptr = (_type)(_len+(char *)_p->ptr))


#define PBUF_PTR(_p, _len)  (_len+(char *)_p->ptr)

#define PRINTF_PKT(_p) \
    do{ \
         int _i; \
         for(_i=0; _i<p->len; _i++)\
         {\
            if(_i%15==0)\
            {\
                printf("\n");\
            }\
            printf("%02x ", *((uint8_t *)p->ptr + _i) );\
         }\
     }while(0)


//#define BYTE_ORDER  LITTLE_ENDIAN
#define __LITTLE_ENDIAN_BITFIELD

#define HEADER_OUTER  0
#define HEADER_INNER  1



//#define DEBUG_ENABLE
#ifdef  DEBUG_ENABLE

#define DEBUG_PRINTF(format, args...) printf(format, ##args) 
#else
#define DEBUG_PRINTF(format, ...)

#endif

#include "pid_ethernet.h"
#include "pid_outerIp4.h"
#include "pid_l4.h"
#include "pid_innerIp4.h"
#include "pid_gre.h"
#include "pid_http.h"
#include "rte_mbuf.h"
#include "naga_stat.h"

berr naga_pid_dpdk(struct rte_mbuf *m);

#endif

