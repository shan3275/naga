/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-03 22:58
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"
#include "naga_types.h"
#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"
#include "upush.h"
//#include "raw_udp.h"

uint8_t  rpush_sip_conf_enable = 0;  // if 1,use configured sip; if 0, use packet sip;
uint32_t rpush_sip = 0; //sip ip 
uint32_t rpush_ip  = 0; //dest ip
uint16_t rpush_port = 0; //dest port

uint32_t wbpush_ip  = 0; //dest ip
uint16_t wbpush_port = 0; //dest port

//#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  printf("func=%s line=%d:" fmt , __FUNCTION__, __LINE__, ##args)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

#define LITTLE_TO_BIG(_ip) \
           _ip =   ( (_ip & 0xff000000) >> 24) \
             | ( (_ip & 0x00ff0000) >> 8 ) \
             | ( (_ip & 0x0000ff00) << 8 ) \
             | ( (_ip & 0x000000ff) << 24)

berr rpush_rule_add(char *s_str, char *p_str)
{
    uint32_t ip    = 0;
    uint16_t port = 0;
    struct prefix p;
    int ret;
    if (s_str == NULL || p_str == NULL)
    {
        return E_PARAM;
    }

    ret = str2prefix (s_str, &p);
    if (ret <= 0)
    {
        return CMD_WARNING;
    }
    ip =((struct prefix_ipv4 *) &p)->prefix.s_addr;
    debug("ip: %d.%d.%d.%d\n", (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8 ) & 0xff,
            (ip >> 0 ) & 0xff);
    LITTLE_TO_BIG(ip);
    debug("ip: %d.%d.%d.%d\n", (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8 ) & 0xff,
            (ip >> 0 ) & 0xff);

    port = atoi(p_str);
    debug("port:%u\n", port);

    rpush_ip   = ip;
    rpush_port = port;

    return E_SUCCESS;
}

berr rpush_rule_del(void)
{
    rpush_ip   = 0;
    rpush_port = 0;

    return E_SUCCESS;
}

berr rpush_rule_get(uint32_t *sip, uint8_t *sip_enable, uint32_t *dip, uint16_t *dport)
{
    *sip = rpush_sip;
    *sip_enable = rpush_sip_conf_enable;
    *dip = rpush_ip;
    *dport = rpush_port;
    return E_SUCCESS;
}

berr rpush_rule_test(void)
{
    berr rv;
    if (rpush_ip == 0 || rpush_port == 0)
    {
        debug("rpush_ip ==0 or rpush_port ==0\n");
        return E_PARAM;
    }
    rv = raw_udp_test();
    if (rv != E_SUCCESS)
    {
        debug("raw_udp_test rv=%d\n", rv);
        return rv;
    }

    return E_SUCCESS;
}

berr rpush_sip_enable_set(const char *en_str)
{
    if ('e' == en_str[0])
    {
        rpush_sip_conf_enable = RPUSH_SIP_ENABLE;
    }
    else
    if ('d' == en_str[0])
    {
        rpush_sip_conf_enable = RPUSH_SIP_DISABLE;
    }
    else
    {
        debug("parameter err %s", en_str);
        return E_PARAM;
    }
    debug("rpush_sip_conf_enable:%d\n", rpush_sip_conf_enable);

    return E_SUCCESS;
}

berr rpush_rule_sip_add(char *s_str)
{
    uint32_t ip    = 0;
    struct prefix p;
    int ret;
    if (s_str == NULL)
    {
        return E_PARAM;
    }

    ret = str2prefix (s_str, &p);
    if (ret <= 0)
    {
        return CMD_WARNING;
    }
    ip =((struct prefix_ipv4 *) &p)->prefix.s_addr;
    debug("ip: %d.%d.%d.%d\n", (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8 ) & 0xff,
            (ip >> 0 ) & 0xff);
    LITTLE_TO_BIG(ip);
    debug("ip: %d.%d.%d.%d\n", (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8 ) & 0xff,
            (ip >> 0 ) & 0xff);


    rpush_sip   = ip;

    return E_SUCCESS;
}


berr wbpush_rule_add(char *s_str, char *p_str)
{
    uint32_t ip    = 0;
    uint16_t port = 0;
    struct prefix p;
    int ret;
    if (s_str == NULL || p_str == NULL)
    {
        return E_PARAM;
    }

    ret = str2prefix (s_str, &p);
    if (ret <= 0)
    {
        return CMD_WARNING;
    }
    ip =((struct prefix_ipv4 *) &p)->prefix.s_addr;
    debug("ip: %d.%d.%d.%d\n", (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8 ) & 0xff,
            (ip >> 0 ) & 0xff);
    LITTLE_TO_BIG(ip);
    debug("ip: %d.%d.%d.%d\n", (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8 ) & 0xff,
            (ip >> 0 ) & 0xff);

    port = atoi(p_str);
    debug("port:%u\n", port);

    wbpush_ip   = ip;
    wbpush_port = port;

    return E_SUCCESS;
}

berr wbpush_rule_del(void)
{
    wbpush_ip   = 0;
    wbpush_port = 0;

    return E_SUCCESS;
}

berr wbpush_rule_get(uint32_t *sip, uint8_t *sip_enable, uint32_t *dip, uint16_t *dport)
{
    *sip = rpush_sip;
    *sip_enable = rpush_sip_conf_enable;
    *dip = wbpush_ip;
    *dport = wbpush_port;
    return E_SUCCESS;
}

berr wbpush_rule_test(void)
{
    berr rv;
    if (wbpush_ip == 0 || wbpush_port == 0)
    {
        debug("rpush_ip ==0 or rpush_port ==0\n");
        return E_PARAM;
    }
    rv = wb_raw_udp_test();
    if (rv != E_SUCCESS)
    {
        debug("raw_udp_test rv=%d\n", rv);
        return rv;
    }

    return E_SUCCESS;
}
