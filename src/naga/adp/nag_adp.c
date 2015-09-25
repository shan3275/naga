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


#if 0

    txm = rte_pktmbuf_alloc(l2fwd_pktmbuf_pool);

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
    itf_send_packet(txm, txm->port);



    txm = rte_pktmbuf_alloc(l2fwd_pktmbuf_pool);

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
    itf_send_packet(txm, txm->port);
#else


#endif
    txm = hytag->m;
    rv = ads_response_head_generator(txm, hytag);
    if(rv != E_SUCCESS)
        return rv;
    printf("send1 buf\n");
    itf_send_packet(txm, txm->port);

    rv = ads_response_content_generator(txm, hytag);
    if(rv != E_SUCCESS)
       return rv;

    printf("send2 buf\n");
    itf_send_packet(txm, txm->port);
    return E_SUCCESS;
}
