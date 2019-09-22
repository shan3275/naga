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
#include "redispush.h"
#include "hiredis.h"
#include "redispush.h"

uint32_t redispush_ip = 0; //server ip 
uint16_t redispush_port = 0; //server port
redisContext *c;

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

berr redispush_rule_add(char *s_str, char *p_str)
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

    redispush_ip   = ip;
    redispush_port = port;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(s_str, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            debug("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            debug("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    return E_SUCCESS;
}

berr redispush_rule_del(void)
{
    redispush_ip   = 0;
    redispush_port = 0;

    /* Disconnects and frees the context */
    redisFree(c);

    return E_SUCCESS;
}

berr redispush_rule_get(uint32_t *sip, uint16_t *dport)
{
    *sip = redispush_ip;
    *dport = redispush_port;
    return E_SUCCESS;
}

berr redispush_rule_test(char * buff)
{
    berr rv;
    redisReply *reply;
    if (redispush_ip == 0 || redispush_port == 0)
    {
        debug("redispush_ip ==0 or redispush_port ==0\n");
        return E_PARAM;
    }
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        return E_FAIL;
    }

    /* PING server */
    reply = redisCommand(c,"PING");
    sprintf(buff, "PING: %s", reply->str);
    freeReplyObject(reply);

    return E_SUCCESS;
}

berr RedisStore(uint8_t status, char * ucIp, char* ucUserName)
{
    if(ucIp == NULL || ucUserName == NULL)
    {
        CNT_INC(ACL_RADIUSPUSH_PARA_ERROR);
        return E_PARAM;
    }

    char command[16] = {0};
    char command1[128] = {0};
    char command2[128] = {0};
    sprintf(command,  "select 15");
    if (status != 2)
    {
        CNT_INC(ACL_RADIUSPUSH_SET);
        sprintf(command1, "hset cxxIPUA  %s  %s", ucIp, ucUserName);
        sprintf(command2, "hset cxxNewIPAD  %s  %s", ucIp, ucUserName);        
    }
    else
    {
        CNT_INC(ACL_RADIUSPUSH_DEL);
        sprintf(command1, "hdel cxxIPUA  %s", ucIp);
        sprintf(command2, "hdel cxxNewIPAD  %s", ucIp); 
    }

    if (REDIS_OK != redisAppendCommand(c,command)||REDIS_OK != redisAppendCommand(c,command1)||REDIS_OK != redisAppendCommand(c,command2))
    {
        CNT_INC(ACL_RADIUSPUSH_PIPE_ERROR);
        return E_FAIL;
    }
    redisReply* reply = NULL;
    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        CNT_INC(ACL_RADIUSPUSH_EXEC_ERROR);
        freeReplyObject(reply);
    }
    freeReplyObject(reply);

    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        CNT_INC(ACL_RADIUSPUSH_EXEC_ERROR);
        freeReplyObject(reply);
    }
    freeReplyObject(reply);

    if (REDIS_OK != redisGetReply(c,(void**)&reply)) {
        CNT_INC(ACL_RADIUSPUSH_EXEC_ERROR);
        freeReplyObject(reply);
    }
    freeReplyObject(reply);

    return E_SUCCESS;
}