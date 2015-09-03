#include "pid.h"
#include <string.h>

berr naga_dpdk_process(struct rte_mbuf *m)
{
    struct pbuf packetp;
    hytag_t hytag ;
    berr errcode;
    //struct pbuf *p = &packetp;
    memset(&packetp, 0x0, sizeof(packetp));
    memset(&hytag, 0x0, sizeof(hytag));
    
    packetp.ptr = rte_pktmbuf_mtod(m, void *);
    packetp.len = m->data_len;
    packetp.ptr_offset = 0;

    DEBUG_PRINTF("enter naga\n");

    errcode =  pid_ethernet(&packetp, &hytag);

    if(errcode)
    {
        pid_incr_count(ERR_PACKET);
        //PRINTF_PKT(p);
        //sleep(100);
    }
    
    return errcode;
}

