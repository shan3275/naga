/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr_cmd_api.c
#       @date         :2015/08/21 22:42
#       @algorithm    :
=============================================================================*/

berr rule_vsr_cmd_add(uint32_t index, uint32_t ip, uint32_t msisdn)
{
    int rv;
    /* parameter check */

    /* del old rule */
    rv = vsr_api_del_rule_by_index(index);
    if(rv)
    {
        /* error del */
    }

    /* add new rule */
    rv = vsr_api_add_rule(index, ip, msisdn);
    if(rv)
    {
        /* error del */
    }

    return E_SUCCESS;

}
berr rule_vsr_cmd_del(uint32_t index)
{
    int rv;
    /* parameter check */

    /* del old rule */
    rv = vsr_api_del_rule_by_index(index);
    if(rv)
    {
        /* error del */
    }

    return E_SUCCESS;

}

berr rule_vsr_cmd_get_index_by_ip(uint32_t ip, uint32_t *index)
{
    int i;
    /* check ip parameter */

    /* check  index pointer */

    /* loop to find out the matched ip */
    for (i = 0; I < VSR_RULE_NUM_MAX; i++)
    {
        if ( ip == vsr_api_get_ip_by_index(i))
        {
            break;
        }
    }

    if ( i == VSR_RULE_NUM_MAX)
    {
        return E_FOUND;
    }
    *index = i;

    return E_SUCCESS;
}


berr_rule_vsr_cmd_get_index_by_mobile(uint32_t mobile, uint32_t *index)
{
    int i;
    /* check mobile parameter */

    /* check  index pointer */

    /* loop to find out the matched ip */
    for (i = 0; I < VSR_RULE_NUM_MAX; i++)
    {
        if ( mobile == vsr_api_get_mobile_by_index(i))
        {
            break;
        }
    }

    if ( i == VSR_RULE_NUM_MAX)
    {
        return E_FOUND;
    }
    *index = i;

    return E_SUCCESS;

}

berr rule_vsr_cmd_dump(uint32_t index, uint8_t *buff)
{
    int rv;
    vsr_rule_entry rule_entry;
    /* check index parameter */

    /* check buff */

    /*get rule entry content*/
    rv = vsr_api_get_rule_entry(index, &rule_entry);
    if (rv)
    {
        return rv;
    }

    /*dump data to buff */
    return E_SUCCESS;
}


berr rule_vsr_cmd_flush_url(uint32_t index)
{
    int rv;
    /* index parameter check */

    /*flush url*/
    rv = vsr_api_flush_url(index);
    if(rv)
    {
    }

    return E_SUCCESS;
}

berr rule_vsr_cmd_clear_statistics(uint32_t index)
{
    int rv;

    /* index parameter check */

    /*clear statistics */
    rv = vsr_api_clear_statistics(index);
    if (rv)
    {
    }

    return E_SUCCESS;

}


