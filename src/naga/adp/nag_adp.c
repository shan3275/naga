#include "boots.h"
#include "bts_debug.h"

#include "naga_types.h"
#include "nag_adp.h"
#include "itf.h"
#include "packet.h"
#include "adp_cmd.h"


#define DEBUG
#ifdef  DEBUG   
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else   
#define debug(fmt,args...)   
#endif  /* DEBUG */ 

uint64_t  g_adp_cnt = 0;
uint64_t  g_adp_interval = 1;
uint64_t  g_adp_success=0;

berr adp_set_interval(int interval)
{
    g_adp_interval = interval;
    return E_SUCCESS;
}

berr adp_get_interval(int *interval, uint64_t *adp_cnt, uint64_t *success)
{
    *interval = g_adp_interval ;
    *adp_cnt = g_adp_cnt;
    *success = g_adp_success;
    return E_SUCCESS;
}





extern struct rte_mempool * l2fwd_pktmbuf_pool;
berr naga_adp(hytag_t *hytag)
{

    berr rv;
	char *rear = NULL;
    struct rte_mbuf *txm = NULL; 

    CNT_INC(ADP_IPKTS);

    if(( NULL == hytag) || (NULL == hytag->m))
    {
        CNT_INC(ADP_DROP_PARAM);
        BRET(E_PARAM);
    }

    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(ADP_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    /* */
    if(!strcmp("www.121zou.com", (char *)hytag->host))
    {
        CNT_INC(ADP_DROP_121ZOU);
        return E_SUCCESS;
    }


    if(strcmp("www.hao123.com", (char *)hytag->host))
    {
        CNT_INC(ADP_DROP_NOT_HAO123);
        return E_SUCCESS;
    }
    else
    {
        CNT_INC(ADP_HAO123);
    }

#if 0
	if(0 == (hytag->acl.actions & ACT_HTTP_RESPONSE))
	{
        CNT_INC(ADP_DROP_ACT_HTTP_RESPONSE);
        return E_SUCCESS;
	}
#endif 
    if(NULL != strstr(hytag->uri, "?_t=t"))
    {
        CNT_INC(ADP_DROP_OUR_SUFFIX);
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
            CNT_INC(ADP_DROP_BACKSLASH_SUFFIX);
			return E_SUCCESS;
        }							
	}
	else
	{
		return E_SUCCESS;
		if( strcmp(rear, ".html") &&  strcmp(rear, ".htm"))
		{
            CNT_INC(ADP_DROP_HTML_SUFFIX);
			return E_SUCCESS;
		}
		
	}

    if(g_adp_cnt % g_adp_interval != 0)
    {
        CNT_INC(ADP_DROP_ADP_INTERVAL);
        return E_SUCCESS;
    }



    txm = hytag->m;

    if(strstr(hytag->user_agent, "MSIE") 
       || strstr(hytag->user_agent, "Macintosh") 
       || ( NULL == strstr(hytag->user_agent, "Phone"))
       
        )
    {
        hytag->template = AD_TEMPLATE_PC;
    }
    else
    {
        hytag->template = AD_TEMPLATE_MOBILE;
    }

    rv = ads_response_head_generator(txm, hytag);
    if(rv != E_SUCCESS) {
        CNT_INC(ADP_DROP_HEAD_GEN1);
        return rv;
    }

    if(hytag->eth_tx == ENABLE)
    {
   
        uint8_t * ptr = rte_pktmbuf_mtod(txm, uint8_t *);
        //printf("prepare to Send packet\n");
        rv = ift_raw_send_packet(hytag->fp, ptr, txm->pkt_len);
        if(rv != E_SUCCESS)
        {
            printf("Send packet Failed\n");
            CNT_INC(ADP_DROP_SEND_PACKET1);
            return rv;
        }
    }  
    else
    {
        itf_send_packet_imm(txm, txm->port);
    }
    
 
#if USE_D_PACKET
   //txm =rte_pktmbuf_clone(txm, txm->pool);
#define CONTENT_FILL_LEN_MAX 1400
   hytag->content_offset = 0;
  /*
   debug("hytag->content_len(%d), hytag->content_offset(%d), hytag->fill_len(%d)", 
           hytag->content_len, hytag->content_offset, hytag->fill_len);
   */
   //usleep(10);
  
   while ( hytag->content_offset < hytag->content_len)
   {
       if ( hytag->content_len - hytag->content_offset >= CONTENT_FILL_LEN_MAX)
       {
           hytag->fill_len = CONTENT_FILL_LEN_MAX;
       }
       else
       {
           hytag->fill_len = hytag->content_len - hytag->content_offset;
       }
       /*
       debug("hytag->content_len(%d), hytag->content_offset(%d), hytag->fill_len(%d)", 
               hytag->content_len, hytag->content_offset, hytag->fill_len);
       */

       rv = ads_response_content_generator(txm, hytag);
       if(rv != E_SUCCESS){

           CNT_INC(ADP_DROP_HEAD_GEN2);
           return rv;
       }

       hytag->content_offset += hytag->fill_len;
       /*
       debug("hytag->content_len(%d), hytag->content_offset(%d), hytag->fill_len(%d)", 
               hytag->content_len, hytag->content_offset, hytag->fill_len);

       */
       if(hytag->eth_tx == ENABLE)
       {
           uint8_t * ptr = rte_pktmbuf_mtod(txm, uint8_t *);

           rv = ift_raw_send_packet(hytag->fp, ptr, txm->pkt_len);

           if(rv != E_SUCCESS)
           {
               CNT_INC(ADP_DROP_SEND_PACKET2);
               printf("Send packet Failed\n");
               return rv;
           }          
       }
       else
       {
           itf_send_packet_imm(txm, txm->port);
       }
       
   }

#endif
    //printf("url: <%s> url_len=%d\n", hytag->url, hytag->url_len);

    g_adp_success++;
    hytag->ad_act = AD_SUCCESS;
    

    CNT_INC(ADP_PUSH_SUCCESS);
    return E_SUCCESS;
}


berr adp_dp_init(void)
{   
   //berr rv; 
   cmdline_adp_init();
   return E_SUCCESS;
}
