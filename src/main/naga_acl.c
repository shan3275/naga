#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "naga_util.h"
#include "naga_acl.h"

#include "packet.h"
#include "itf.h"
#include "upush.h"
#include "raw_udp.h"
#include "dbg_tagmon.h"

static int urlencode(char* str, int strSize, char* result, int resultSize)
{
    int i;
    int j = 0; /* for result index */
    char ch;

    if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0)) {
        return 0;
    }

    for (i=0; (i<strSize) && (j<resultSize); i++) {
        ch = str[i];
        if ((ch >= 'A') && (ch <= 'Z')) {
            result[j++] = ch;
        } else if ((ch >= 'a') && (ch <= 'z')) {
            result[j++] = ch;
        } else if ((ch >= '0') && (ch <= '9')) {
            result[j++] = ch;
        } else if(ch == ' '){
            result[j++] = '+';
        } else {
            if (j + 3 < resultSize) {
                sprintf(result+j, "%%%02X", (unsigned char)ch);
                j += 3;
            } else {
                return 0;
            }
        }
    }
    result[j] = '\0';
    return j;
}

static berr naga_acl_redir(hytag_t *hytag)
{

    berr rv;
    unsigned char buffer[2048]; 
    char url[2048];
    char all_url[512] = {0};
    char encode_url[512] = {0};
    //uint16_t encode_url_len;

    CNT_INC(ACL_REDIR_PKTS);

    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(ACL_REDIR_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    if (hytag->url_len > 512)
    {
        CNT_INC(ACL_REDIR_LONG_URI_DROP);
        return E_EXCEED; 
    }

    CYCLE_START();

    memcpy(buffer, hytag->pbuf.ptr, hytag->l5_offset);//copy l2-l4 len

    snprintf(all_url, 512, "http://%s", hytag->url);
    urlencode(all_url, strlen(all_url), encode_url, 512); 
    memset(url, 0, 2048);
    strcpy(url, hytag->acl.url);
    snprintf(url, 2048, "%s?ink=%s", hytag->acl.url, encode_url);
    rv = redirect_302_response_generator(buffer, hytag, url); 


    if(rv != E_SUCCESS) 
    {
        CNT_INC(ACL_REDIR_DROP_HEAD_GEN1);
        return rv;
    }


    CYCLE_END();

    CYCLE_START();
    rv = ift_raw_send_packet(buffer, hytag->data_len);
    if(rv != E_SUCCESS)
    {
        CNT_INC(ACL_REDIR_DROP_SEND_PACKET);
        return rv;
    }

    CYCLE_END();
  

   
   CNT_INC(ACL_REDIR_TX_SUCCESS);
   return E_SUCCESS;
}

static berr naga_acl_adp(hytag_t *hytag)
{
    berr rv;
    unsigned char buffer[2048]; 
    //char url[2048];

    CNT_INC(ACL_ADP_PKTS);

    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(ACL_ADP_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    if (hytag->url_len > 512)
    {
        CNT_INC(ACL_ADP_LONG_URI_DROP);
        return E_EXCEED; 
    }

    memcpy(buffer, hytag->pbuf.ptr, hytag->l5_offset);//copy l2-l4 len
    rv = ads_response_head_generator(buffer, hytag);
    if(rv != E_SUCCESS) 
    {
        CNT_INC(ACL_ADP_DROP_HEAD_GEN1);
        return rv;
    }

    CYCLE_START();
    rv = ift_raw_send_packet(buffer, hytag->data_len);
    if(rv != E_SUCCESS)
    {
        CNT_INC(ACL_ADP_DROP_SEND_PACKET);
        return rv;
    }

   CNT_INC(ACL_ADP_TX_SUCCESS);
   return E_SUCCESS;

}

static berr naga_acl_udppush(hytag_t *hytag)
{
    berr rv;
    char payload[1024];
    unsigned char buffer[2048]; 
    int len;

    CNT_INC(ACL_UDPPUSH_PKTS);

    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(ACL_UDPPUSH_DROP_GET_OR_POST);
        return E_SUCCESS;
    }
    
    if (hytag->user_agent_len <= 0)
    {
        CNT_INC(ACL_UDPPUSH_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    memset(payload,0, 1024);
    rv = upush_content_generator(hytag, payload);
    if(rv != E_SUCCESS) 
    {
        CNT_INC(ACL_UDPPUSH_FAIL);
        return rv;
    }
    CNT_INC(ACL_UDPPUSH_PAYLOAD_GEN);

    rv = raw_udp_content_generator(buffer, payload, &len, hytag);
    if(rv != E_SUCCESS) 
    {
        CNT_INC(ACL_UDPPUSH_DROP_HEAD_GEN1);
        return rv;
    }
    CNT_INC(ACL_UDPPUSH_RAW_UDP_GEN);

    CYCLE_START();
    rv = ift_raw_send_packet(buffer, len);
    if(rv != E_SUCCESS)
    {
        CNT_INC(ACL_UDPPUSH_DROP_SEND_PACKET);
        return rv;
    }

    CNT_INC(ACL_UDPPUSH_TX_SUCCESS);
    return E_SUCCESS;
}

static berr naga_acl_wb_udppush(hytag_t *hytag)
{
    berr rv;
    char payload[1024];
    unsigned char buffer[2048]; 
    int len;

    CNT_INC(ACL_WBPUSH_PKTS);

    /* */
    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(ACL_WBPUSH_DROP_GET_OR_POST);
        return E_SUCCESS;
    }
    
    if (hytag->user_agent_len <= 0)
    {
        CNT_INC(ACL_WBPUSH_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    memset(payload,0, 1024);
    rv = wb_upush_content_generator(hytag, payload);
    if(rv != E_SUCCESS) 
    {
        CNT_INC(ACL_WBPUSH_FAIL);
        return rv;
    }
    CNT_INC(ACL_WBPUSH_PAYLOAD_GEN);

    rv = wb_raw_udp_content_generator(buffer, payload, &len, hytag);
    if(rv != E_SUCCESS) 
    {
        CNT_INC(ACL_WBPUSH_DROP_HEAD_GEN1);
        return rv;
    }
    CNT_INC(ACL_WBPUSH_RAW_UDP_GEN);

    CYCLE_START();
    rv = ift_raw_send_packet(buffer, len);
    if(rv != E_SUCCESS)
    {
        CNT_INC(ACL_WBPUSH_DROP_SEND_PACKET);
        return rv;
    }

    CNT_INC(ACL_WBPUSH_TX_SUCCESS);
    return E_SUCCESS;
}

static berr naga_acl_urlpush(hytag_t *hytag)
{
    berr rv;
    char ptr[2048]; 

    CNT_INC(ACL_URLPUSH_PKTS);

    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        CNT_INC(ACL_URLPUSH_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    if (hytag->user_agent_len <= 0)
    {
        CNT_INC(ACL_URLPUSH_DROP_GET_OR_POST);
        return E_SUCCESS;
    }

    memset(ptr,0, 2048);
    rv = upush_content_generator(hytag, ptr); 
    if(rv != E_SUCCESS) 
    {
        if (rv == E_ENCODE)
        {
            CNT_INC(ACL_URLPUSH_ENCODE_FAIL);
        }
        else
        if (rv == E_PARAM)
        {
            CNT_INC(ACL_URLPUSH_PARAM_ERR);
        }
        return rv;
    }

    rv = upush_send(ptr);
    if(rv != E_SUCCESS)
    {
        CNT_INC(ACL_URLPUSH_SENDER_NULL);
        return rv;
    }

   return E_SUCCESS;
}

berr naga_acl(hytag_t *hytag)
{
    int rv = E_MAX;

    if (NULL == hytag) 
    {
        return E_SUCCESS;
    }

    if(ACT_UDPPUSH == (hytag->acl.actions & ACT_UDPPUSH))
    {
        CNT_INC(ACL_PKTS);
        rv = naga_acl_udppush(hytag);
        if (rv != E_SUCCESS) {
            //printf("%s.%d: rv = %s\n", __func__, __LINE__, berr_msg(rv));
        }
    }
    else
    if(ACT_WBPUSH == (hytag->acl.actions & ACT_WBPUSH))
    {
        CNT_INC(ACL_PKTS);
        rv = naga_acl_wb_udppush(hytag);
        if (rv != E_SUCCESS) {
            //printf("%s.%d: rv = %s\n", __func__, __LINE__, berr_msg(rv));
        }
    }
    else
    if(ACT_REDIR == (hytag->acl.actions & ACT_REDIR))
    {
        CNT_INC(ACL_PKTS);
        rv = naga_acl_redir(hytag);
        if (rv != E_SUCCESS) {
            //printf("%s.%d: rv = %s\n", __func__, __LINE__, berr_msg(rv));
        }
    }
    else
    if(ACT_URLPUSH == (hytag->acl.actions & ACT_URLPUSH))
    {
        CNT_INC(ACL_PKTS);
        rv = naga_acl_urlpush(hytag);
        if (rv != E_SUCCESS) {
            //printf("%s.%d: rv = %s\n", __func__, __LINE__, berr_msg(rv));
        }
    }
    else
    if(ACT_ADP == (hytag->acl.actions & ACT_ADP))
    {
        CNT_INC(ACL_PKTS);
        rv = naga_acl_adp(hytag);
        if (rv != E_SUCCESS) {
            //printf("%s.%d: rv = %s\n", __func__, __LINE__, berr_msg(rv));
        }
    }

    if(ACT_LOG == (hytag->acl.actions & ACT_LOG))
    {
        CNT_INC(ACL_PKTS);
        hytag_log(hytag);
    }

    return E_SUCCESS;
}

/* End of file */
