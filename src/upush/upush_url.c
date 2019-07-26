/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-03 22:59
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
#include "cbase64.h"

//#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

berr upush_content_generator(hytag_t *hytag, char *ptr)
{
    int outlen =0;
    int rv = 0;
    char buffer[2048];

    memset(buffer,0,sizeof(buffer));
    if (APP_URLPUSH_IDFA == hytag->acl.push_type)
    {
        sprintf( buffer, "{\"flag\":\"10\",\"srcIP\":\"%d.%d.%d.%d\",\"idfa\":\"%s\"}",
                (hytag->outer_srcip4 >>24) &0xff,
                (hytag->outer_srcip4 >>16) &0xff,
                (hytag->outer_srcip4 >>8 ) &0xff,
                (hytag->outer_srcip4     ) &0xff,
                hytag->reg);
        CNT_INC(ACL_URLPUSH_IDFA);
    }
    else
    if (APP_URLPUSH_APPID == hytag->acl.push_type)
    {
        sprintf( buffer, "{\"flag\":\"20\",\"srcIP\":\"%d.%d.%d.%d\",\"appid\":\"%s\",\"dsid\":\"%s\"}",
                (hytag->outer_srcip4 >>24) &0xff,
                (hytag->outer_srcip4 >>16) &0xff,
                (hytag->outer_srcip4 >>8 ) &0xff,
                (hytag->outer_srcip4     ) &0xff,
                hytag->reg,
                hytag->icloud_dsid);
        CNT_INC(ACL_URLPUSH_APPID);
    }
    else
    {
        return E_PARAM;
    }

    debug("buffer(%s),len(%d)", buffer, strlen(buffer));

    rv =  base64_encode(buffer, strlen(buffer), ptr, &outlen);
    if (rv)
    {
        return E_ENCODE;
    }
    debug("encode buff: %s", ptr);

    return E_SUCCESS;
}


berr wb_upush_content_generator(hytag_t *hytag, char *ptr)
{
    int outlen =0;
    int rv = 0;
    char buffer[2048];

    memset(buffer,0,sizeof(buffer));
    if (hytag->wb_from_len)
    {
        sprintf( buffer, "%s;%s;%s;%s;%s;%s;%s",
                hytag->wb_i,
                hytag->wb_s,
                hytag->wb_ua,
                hytag->wb_aid,
                hytag->wb_gsid,
                hytag->wb_uid,
                hytag->wb_from); 
    }
    else
    {
        sprintf( buffer, "%s;%s;%s;%s;%s;%s",
            hytag->wb_i,
            hytag->wb_s,
            hytag->wb_ua,
            hytag->wb_aid,
            hytag->wb_gsid,
            hytag->wb_uid);        
    }
    debug("buffer(%s),len(%d)", buffer, strlen(buffer));

    rv =  base64_encode(buffer, strlen(buffer), ptr, &outlen);
    if (rv)
    {
        return E_ENCODE;
    }
    debug("encode buff: %s", ptr);

    return E_SUCCESS;
}
