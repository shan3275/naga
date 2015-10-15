/**************************************************************
* Copyright (C) 2014-2015 All rights reserved.
* @Version: 1.0
* @Created: 2015-10-15 07:06
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include "bts_cnt.h"
#include "netseg.h"
berr netseg_dp_match(uint32_t ip)
{
    int i;
    int rv = E_SUCCESS;
    for ( i = 0; i < NETSEG_RULE_NUM_MAX; i++)
    {
        /* match success */
        rv = api_net_dp_match(i,ip);
        if(rv == E_SUCCESS)
        {
            break;
        }
    }
    return rv;
}

berr netseg_dp_process(hytag_t *hytag)
{
    int rv;

    if ( NULL == hytag )
    {
        return E_PARAM;
    }

    /*add recv statistics */
    cnt_inc(NET_PKTS);

    /*check protocol type*/
    if ( hytag->app_type != APP_TYPE_HTTP_GET_OR_POST)
    {
        /* add statistics */
        cnt_inc(NET_UNURLPKTS);
        return E_SUCCESS;
    }

    /* add IP_UDP_GTP_IP_URL packet statistics */
    cnt_inc(NET_URLPKTS);

    /* IP_UDP_GTP_IP_URL packet process */
    rv = net_dp_match(hytag->outer_srcip4);
    if(rv)
    {
        /*add not match statistics */
        cnt_inc(NET_UNMATCHPKTS);
        hytag->match &= 0xfffe;
    }
    else
    {
        /* add match statistics */
        cnt_inc(NET_MATCHPKTS);
        hytag->match |= 1;
    }
    return E_SUCCESS; 
}
