#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "naga_util.h"
#include "naga_acl.h"

#include "packet.h"
#include "itf.h"

berr naga_acl_redir(hytag_t *hytag)
{

    berr rv;
    unsigned char buffer[2048]; 
    char url[2048];

    if (NULL == hytag) 
    {
        return E_PARAM; 
    }

    if(ACT_REDIR != (hytag->acl.actions & ACT_REDIR))
    {
        return E_SUCCESS;
    }

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

    memset(url, 0, 2048);
    strcpy(url, hytag->acl.url);
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


berr naga_acl(hytag_t *hytag)
{
    int rv = E_MAX;

    rv = naga_acl_redir(hytag);

    if (rv != E_SUCCESS) {
        printf("%s.%d: rv = %s\n", __func__, __LINE__, berr_msg(rv));
    }

    return E_SUCCESS;
}

/* End of file */
