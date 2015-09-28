#include "naga_types.h"
#include "nag_adp.h"
#include "itf.h"
#include "packet.h"


extern struct rte_mempool * l2fwd_pktmbuf_pool;
berr naga_adp(hytag_t *hytag)
{
    
    berr rv;
	char *rear = NULL;
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


    if(NULL != strstr(hytag->uri, "?_t=t"))
    {
	    return E_SUCCESS;
    }
	rear= strrchr(hytag->uri, '.');

	if(rear == NULL)
	{
	    if(hytag->uri_len == 1 && !strcmp(hytag->uri, "/"))
	    {
	    }						
	    else
	    {
		return E_SUCCESS;
            }							
	}
	else
	{
		return E_SUCCESS;
		if( strcmp(rear, ".html") &&  strcmp(rear, ".htm"))
		{
			return E_SUCCESS;
		}
		
	}







    txm = hytag->m;
    rv = ads_response_head_generator(txm, hytag);
    if(rv != E_SUCCESS)
        return rv;
    //printf("send1 buf\n");

    
    if(hytag->eth_tx== ENABLE)
    {
        uint8_t * ptr = rte_pktmbuf_mtod(txm, uint8_t *);
        rv = ift_raw_send_packet(hytag->fp, ptr, txm->pkt_len);
        if(rv != E_FAIL)
            return rv;
    }
    else
    {
        itf_send_packet_imm(txm, txm->port);
    }
    
    printf("url: <%s> url_len=%d", hytag->url, hytag->url_len);




// sleep(1);
/*
    rv = ads_response_content_generator(txm, hytag);
    if(rv != E_SUCCESS)
       return rv;

    printf("send2 buf\n");
    itf_send_packet_imm(txm, txm->port);*/

    
    return E_SUCCESS;
}
