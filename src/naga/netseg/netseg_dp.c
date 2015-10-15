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
berr netseg_dp_match(uint32_t ip, int *index)
{
    int i;
    int rv = E_SUCCESS;
	netseg_t *seg = NULL;

	seg = api_get_netseg_ptr();
	if (NULL == seg)
	{
		return E_FAIL;
	}
	
    for ( i = 0; i < NETSEG_RULE_NUM_MAX; i++)
    {
        /* match success */
        //rv = api_net_dp_match(i,ip);
        //if(rv == E_SUCCESS)
        //{
        //	*index = i;
        //    break;
        //}

		if(seg[i].effective == NETSEG_RULE_UNEFFECTIVE)
		{
			continue;			
		}
		if ( ip & seg[i].net.mask != seg[i].net.ip & seg[i].net.mask )
		{
			continue;	
		}
		else
		{
			*index = i;
			return E_SUCCESS;
		}
		
		

		
    }
    return E_NULL;
}

berr netseg_dp_process(hytag_t *hytag)
{
    int rv;
	net_t rule;
	int index = 0;

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
    rv = netseg_dp_match(hytag->outer_srcip4, &index);
    
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
		memset(&rule, 0, sizeof(net_t));
		rv = api_net_get(index, &rule);
		if (rv)
		{
			return E_FAIL;
		}
		ACL_HIT(rule.acl);
        HYTAG_ACL_MERGE(hytag->acl, rule.acl);
    }
    return E_SUCCESS; 
}
