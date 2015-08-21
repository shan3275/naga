/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_dp.c
#       @date         :2015/08/21 22:21
#       @algorithm    :
=============================================================================*/

berr vsr_dp_match(uint32_t ip, char*url)
{
    int i;
    int rv = E_SUCCESS;
    for ( i = 0; i < VSR_RULE_NUM_MAX; i++)
    {
        /* match success */
        rv = vsr_dp_api_match(i,ip, usr);
        if(rv == E_SUCCESS)
        {
            break;
        }
    }
    return rv;
}

berr vsr_dp_api_match(uint32_t index, uint32_t ip, char* url)
{
    int rv = E_SUCCESS;
    uint32_t hash = 0;
    uint32_t len = 0;
    int i;
    uint32_t url_num = 0;
    /* lock */
    /* check rule effective */
    if(vsr_check_rule_effective(index)== VSR_RULE_UNEFFECTIVE)
    {
        return E_NULL;
    }

    /* check ip */
    if ( ip != vsr_get_rule_ip(index))
    {
        return E_MATCH;
    }
    /* get len of the url */
    len = ;
    /* caluate the hash value of url */
    hash = vsr_hash(url, len);
    /* search url */
    url_num = vsr_get_rule_url_num(index);
    for( i = 0; i < url_num; i++)
    {
        /*compare hash */
        if(hash != vsr_get_url_hash(index,i))
        {
            continue;
        }
        /* compare len*/
        if(len != vsr_get_url_len(index, i))
        {
            continue;
        }
        /* compare url */
        if(strncmp(url, vsr_get_url_content(index,i), len))
        {
            continue;
        }
        /* all matched, so break*/
        /* add match ip statistics */
        break;
    }

    /* match, just update the statistics */
    if(url_num)
    {
        if(i < url_num)
        {
            vsr_url_pkt_inc(index, i);
        }
    }
    /* not match, insert the url content */
    if ( i == url_num)
    {
        /* update hash*/
        /* update content */
        /* update the statitics */
        /* url_num ++*/
    }


    /* unlock */

    return rv;
}

berr vsr_dp_process(hytag_t *hytag)
{
    int rv;
    /*add recv statistics */
    /* code */

    /*check protocol type*/
    if ( hytag->protocol_type != IP_UDP_GTP_IP_URL)
    {
        /* add statistics */
        /* code */
        return E_SUCCESS;
    }

    /* add IP_UDP_GTP_IP_URL packet statistics */
    /* code */

    /* IP_UDP_GTP_IP_URL packet process */
    rv = vsr_dp_match(hytag->inner_srcip, hytag->url);
    if(rv)
    {
        /*add not match statistics */
    }
    else
    {
        /* add match statistics */
    }
    return E_SUCCESS; 
}
