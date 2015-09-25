#include "naga_types.h"
#include "nag_adp.h"
#include "itf.h"
#include "packet.h"


extern struct rte_mempool * l2fwd_pktmbuf_pool;
berr naga_adp(hytag_t *hytag)
{
    
    berr rv;
    struct rte_mbuf *txm = NULL; 
    
    if( NULL == hytag )
    {
        return E_PARAM;
    }
    if(NULL == hytag->m)
    {
        return E_PARAM;
    }

    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        return E_SUCCESS;
    }
    if(!strcmp("www.121zou.com", (char *)hytag->host))
    {
         return E_SUCCESS;
    }

 #if 0   


    txm = rte_pktmbuf_clone(hytag->m, l2fwd_pktmbuf_pool);

    if( NULL == txm)
    {
        printf("Tx Failed For mem alloc\n");
        return E_SUCCESS;
    }



    txm->port = hytag->m->port;
    uint8_t *dst = rte_pktmbuf_mtod(txm, void *);
    uint8_t *src = rte_pktmbuf_mtod(hytag->m, void *);
    
    memcpy((void *)dst, (void *)src, hytag->m->data_len);
    
    
    rv = ads_response_head_generator(txm, hytag);
    if(rv != E_SUCCESS)
        return rv;
    printf("send1 buf\n");
    printf("len = %d, %d, %d\n", txm->data_len, txm->buf_len, txm->pkt_len);    
    itf_send_packet_imm(txm, txm->port);


    txm = rte_pktmbuf_clone(hytag->m, l2fwd_pktmbuf_pool);

    if( NULL == txm)
    {
        printf("Tx Failed For mem alloc\n");
        return E_SUCCESS;
    }

    txm->port = hytag->m->port;
    dst = rte_pktmbuf_mtod(txm, void *);
    src = rte_pktmbuf_mtod(hytag->m, void *);
    
    memcpy((void *)dst, (void *)src, hytag->m->data_len); 
    
    rv = ads_response_content_generator(txm, hytag);
    if(rv != E_SUCCESS)
       return rv;

    printf("send2 buf\n");
    itf_send_packet_imm(txm, txm->port);
    
#else



    
    txm = hytag->m;
    printf("len = %d, %d, %d\n", txm->data_len, txm->buf_len, txm->pkt_len);
    rv = ads_response_head_generator(txm, hytag);
    if(rv != E_SUCCESS)
        return rv;
    printf("send1 buf\n");
    itf_send_packet_imm(txm, txm->port);

    sleep(1);
/*
    rv = ads_response_content_generator(txm, hytag);
    if(rv != E_SUCCESS)
       return rv;

    printf("send2 buf\n");
    itf_send_packet_imm(txm, txm->port);*/
#endif
    
    return E_SUCCESS;
}
