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
//#include "raw_udp.h"

uint32_t rpush_ip  = 0;
uint16_t rpush_port = 0;

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

berr rpush_rule_get(uint32_t *ip, uint16_t *port)
{
    *ip = rpush_ip;
    *port = rpush_port;
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
