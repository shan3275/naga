/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_api.c
#       @date         :2015/08/21 22:42
#       @algorithm    :
=============================================================================*/

berr vsr_api_del_rule_by_index(uint32_t index)
{
    uint32_t url_num = 0;
    int i;
    /* check effective */
    if(vsr_check_rule_effective(index)== VSR_RULE_UNEFFECTIVE)
    {
        return E_SUCCESS;
    }

    /*clear ip, mobile, effective */
    vsr_set_rule_ip(index, VSR_RULE_IP_UNEFFECTIVE);
    vsr_set_rule_mobile(index, VSR_RULE_MOBILE_UNEFFECTIVE);
    vsr_set_rule_effective(index, VSR_RULE_UNEFFECTIVE);

    /* loop url_num*/
    url_num = vsr_get_url_num(index);
    for(I = 0; I <= url_num; i++)
    {
        /*check url effective */
        if(vsr_check_url_effective(index, i) == VSR_RULE_URL_UNEFFECTIVE )
        {
            /* abnormal deal */
        }

        vsr_set_url_len(index, i, VSR_RULE_URL_LEN_UNEFFECTIVE);
        vsr_set_url_hash(index, i, VSR_RULE_URL_HASH_UNEFFECTIVE);
        vsr_set_url_content(index, i, 0, NULL);
        vsr_set_url_pkt(index, i, 0);
    }

    /* clear url_num */
    vsr_set_rule_url_num (index, VSR_RULE_URL_NUM_UNEFFECTIVE);

    /* ip num --*/
    vsr_ip_num_dec();
    return E_SUCCESS;
}


berr vsr_api_add_rule(uint32_t index, uint32_t ip, uint32_t msisdn)
{
    /* set ip, mobile, and effective */
    vsr_set_rule_ip(index, ip);
    vsr_set_rule_msisdn(index, msisdn);
    vsr_set_rule_effective(index, VSR_RULE_EFFECTIVE);

    /* ip_num ++*/
    vsr_ip_num_add();

    return E_SUCCESS;
}


/*
 *  * input: index, value form 0-15
 *   * return : ip
 *    */
uint32_t vsr_api_get_ip_by_index(uint32_t index)
{
    uint32_t ip = 0;
    /* lock */

    ip = vsr_get_rule_ip(index);
    /* unlock */

    return ip;
}

/*
 *  * input: index, value form 0-15
 *   * return : mobile
 *    */
uint32_t vsr_api_get_mobile_by_index(uint32_t index)
{
    uint32_t mobile = 0;
    /* lock */

    mobile = vsr_get_rule_mobile(index);
    /* unlock */

    return mobile;
}

berr vsr_api_get_rule_entry(uint32_t index, vsr_rule_entry *rule)
{
    /*lock */

    /*check rule whether effective or not */
    if(vsr_check_rule_effective(index)== VSR_RULE_UNEFFECTIVE)
    {
        return E_NULL;
    }

    /*copy to */
    memcpy(rule, vsr_get_rule_entry(index), sizeof(vsr_rule_entry));

    /*unlock */

    return E_SUCCESS;
}

berr vsr_api_flush_url(uint32_t index)
{
    /* lock */

    /*check rule effective */

    /* get rule url num */

    /* loop clear url */

    /* unlock */
    return E_SUCCESS;

}

berr vsr_api_clear_statistics(uint32_t index)
{
    /*lock */

    /*clear statistics*/
    /*clear rule ip match pkt*/

    /* loop clear url match pkt*/

    /*unlock */
    return E_SUCCESS;
}

