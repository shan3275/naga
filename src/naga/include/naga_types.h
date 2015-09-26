#ifndef __NAGA_TYPES_H__
#define __NAGA_TYPES_H__

#include "boots.h"

typedef uint32_t ipaddr_t;

#define NAGA_ACCOUNT_STR_SZ 32


#define ACT_DROP            1
#define ACT_HTTP_RESPONSE   2
#define ACT_LOG             4   
#define ACT_COUNT           8

typedef struct {
    uint32_t actions;
    uint32_t outport;
    bts_atomic64_t cnt;
} naga_acl_t;

#define ACL_DORP(_acl) \
    (_acl).actions |= ACT_DROP

#define ACL_FWD(_acl, _port) \
{ \
    _acl.actions |= ACT_FORWARD; \
    _acl.outport  = _port; \
}

#define ACL_LOG(_acl) \
    _acl.actions |= ACT_LOG

#define ACL_CNT(_acl) \
    _acl.actions |= ACT_COUNT

#define ACL_HIT(_acl) \
    bts_atomic64_inc(&(_acl.cnt))




typedef enum
{
    APP_TYPE_HTTP_GET_OR_POST = 1,
    APP_TYPE_HTTP_OTHER,    
}APP_TYPE_E;

struct pbuf {
	//struct pbuf *next; /*always NULL*/
	void 	* ptr;	
	uint16_t  len;  /*this buffer len*/
	//uint16_t  total_len; 
	uint16_t  ptr_offset;/*when The module Pid handle the protocol any*/
	//uint8_t flag;
};


#define URL_MAX_LEN  512 //URL MAX LEN
#define MAX_HOST_LEN 128

typedef struct
{

    /* L2 offset */
    uint16_t l2_offset;

    /* L3 offset */
    uint16_t l3_offset;
    uint16_t total_len; /* ip packet total length */

    /* L4 offset */
    uint16_t l4_offset;

    /* L5 offset */
    uint16_t l5_offset;
    uint16_t l5_len;

    
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
    uint16_t host_len;
	uint16_t referer_len;
	char url[URL_MAX_LEN];
	uint8_t host[MAX_HOST_LEN];
    char account[NAGA_ACCOUNT_STR_SZ];
	char referer[URL_MAX_LEN];
    naga_acl_t acl;


    char     uri[URL_MAX_LEN];	
    uint16_t uri_len;

    struct pbuf pbuf;
    struct rte_mbuf *m;
    uint16_t match;   /* 0 for not match, 1 for vsr match, 2 for other match  */
}hytag_t;

#define HYTAG_ACL_MERGE(_tagacl, _ruleacl) \
{ \
    (_tagacl).actions |= (_ruleacl).actions; \
    (_tagacl).outport |= (_ruleacl).outport; \
    ACL_HIT(_tagacl);\
}



#endif /* !__NAGA_TYPES_H__ */
