#include "boots_custom.h"
#include "bts_cnt.h"
#include "bts_debug.h"

cnt_t cnt_array[CNT_MAX] =
{
    CNT_DEF(ITF0_IPKTS),
    CNT_DEF(ITF1_IPKTS),
    CNT_DEF(ITF_IPKTS),
    CNT_DEF(ITF_IBYTS),
    CNT_DEF(ITF_OPKTS),
    CNT_DEF(ITF_OBYTS),
    CNT_DEF(VLAN_PKTS),
    CNT_DEF(PPPOE_PKTS),
    CNT_DEF(XF0_PKTS),
    CNT_DEF(XF1_PKTS),
	CNT_DEF(OUTERL3_HD),   /*Head Err, length not compare or infomation err*/
	CNT_DEF(OUTERL3_IPV4),
	CNT_DEF(OUTERL3_IPV6),
	CNT_DEF(OUTERL3_NOIP),

	CNT_DEF(OUTERL4_HD),
	CNT_DEF(OUTERL4_TCP),
	CNT_DEF(OUTERL4_UDP),
	CNT_DEF(OUTERL4_SCTP),

	CNT_DEF(OUTERL4_OTHER),
	CNT_DEF(INNERL3_HD),
	CNT_DEF(INNERL3_IPV4),
	CNT_DEF(INNERL3_IPV6),

	CNT_DEF(INNERL3_NOIP),
	CNT_DEF(INNERL4_HD),
	CNT_DEF(INNERL4_TCP),
	CNT_DEF(INNERL4_UDP),

	CNT_DEF(INNERL4_SCTP),
	CNT_DEF(INNERL4_OTHER),
	CNT_DEF(GTP_U_HD),
	CNT_DEF(GTP_U),

	CNT_DEF(GTP_U_NOPAYLOAD),
	CNT_DEF(GTPC_HD),
	CNT_DEF(GTPV1_C),
	CNT_DEF(GTPV2_C),

	CNT_DEF(APP_HTTP_UP),
	CNT_DEF(APP_HTTP_UP_OVER),
	CNT_DEF(APP_HTTP_DN),
	CNT_DEF(APP_OTHER),
	CNT_DEF(HTTP_METHOD_EXCEED),
	CNT_DEF(APP_HTTP_GET),
	CNT_DEF(APP_HTTP_OTHER),
	CNT_DEF(APP_HTTP_PARSE_ERR),
	CNT_DEF(HTTP_URL_EXCEED),
	CNT_DEF(ERR_PACKET),
	CNT_DEF(HEADER_UNEXPECT),
#ifdef CNT_DEF_CUSTOM
    CNT_DEF_CUSTOM,
#endif
};

rate_t pkts_rate= {0};

berr cnt_add(cnte idx, uint64_t value)
{
    if (idx >= CNT_MAX) 
    {
        BRET(E_EXCEED);
    }

    CNT_ADD(idx, value);

    BRET(E_SUCCESS);
}

berr cnt_inc(cnte idx)
{
    if (idx >= CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    CNT_INC(idx);

    BRET(E_SUCCESS);
}

berr cnt_dec(cnte idx)
{
    if (idx >= CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    CNT_DEC(idx); 

    BRET(E_SUCCESS);
}

berr cnt_get(cnte idx, uint32_t number, cnt_t *vals, uint32_t *total)
{
    uint32_t i  = 0, idxc = 0;

    if ((idx + number) > CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    if ((NULL == vals) || (NULL == total))
    {
        BRET(E_PARAM);
    }

    *total = 0;

    for (i = 0; i < number; i++)
    {
        idxc = idx + i;
        vals[i] = cnt_array[idxc];
        *total += 1;
    }

    BRET(E_SUCCESS);
}

berr cnt_clear(cnte idx, uint32_t number, uint32_t *total)
{
    uint32_t i  = 0, idxc = 0;

    if ((idx + number) > CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    if (NULL == total)
    {
        BRET(E_PARAM);
    }

    *total = 0;

    for (i = 0; i < number; i++)
    {
        idxc = idx + i;
        CNT_SET(idxc, 0); 
        *total += 1;
    }
     //printf("%s.%d\n", __func__, __LINE__);

    BRET(E_SUCCESS);
}

berr cnt_int()
{
    uint32_t i = 0;

    for (i = 0; i < CNT_MAX; i++)
    {
        CNT_SET(i, 0);
    }

    BRET(E_SUCCESS);
}

void pkt_rate_update(void)
{
    uint64_t pkts_now,  pkts_before;
    uint64_t bytes_now, bytes_before;

    pkts_now  = CNT_GET(ITF_IPKTS);
    bytes_now = CNT_GET(ITF_IBYTS);

    pkts_before  = pkts_rate.pkts_before;
    bytes_before = pkts_rate.bytes_before;

    if (pkts_now <= pkts_before || bytes_now <= bytes_before)
    {

        pkts_rate.pkts_rate    = 0;
        pkts_rate.bps_rate     = 0; 
    }
    else
    {
        pkts_rate.pkts_rate    = (pkts_now - pkts_before)/10;
        pkts_rate.bps_rate     = (bytes_now- bytes_before)*8/10; 
    }

    pkts_rate.pkts_before  = pkts_now;
    pkts_rate.bytes_before = bytes_now;
}

berr pkt_rate_get(rate_t **rate)
{
    *rate = &pkts_rate;
    BRET(E_SUCCESS);
}

/* End of file */
