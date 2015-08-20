#include "naga.h"
#include <string.h>

berr naga_pid_dpdk(struct rte_mbuf *m)
{
    struct pbuf packetp;
    hytag_t hytag ;
    berr errcode;

    memset(&packetp, 0x0, sizeof(packetp));
    memset(&hytag, 0x0, sizeof(hytag));
    
    packetp.ptr = rte_pktmbuf_mtod(m, void *);
    packetp.len = m->data_len;
    packetp.ptr_offset = 0;
    
	errcode =  pid_ethernet(&packetp, &hytag);

    return errcode;
}
