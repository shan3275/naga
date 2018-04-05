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
    //get time
    char date[32] = {0};
    struct timeval clock;
    struct tm *tm;
    gettimeofday(&clock, NULL);
    tm = localtime(&clock.tv_sec);
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);

    memset(buffer,0,sizeof(buffer));
    sprintf( buffer, "{\"Time\":\"%s\",\"srcIP\":\"%d.%d.%d.%d\",\"Url\":\"%s\"}",
            date,
            (hytag->outer_srcip4 >>24) &0xff,
            (hytag->outer_srcip4 >>16) &0xff,
            (hytag->outer_srcip4 >>8) &0xff,
            (hytag->outer_srcip4) &0xff,
            hytag->url);

    debug("buffer(%s),len(%d)", buffer, strlen(buffer));

    rv =  base64_encode(buffer, strlen(buffer), ptr, &outlen);
    if (rv)
    {
        return E_ENCODE;
    }
    debug("encode buff: %s", ptr);

    return E_SUCCESS;
}

