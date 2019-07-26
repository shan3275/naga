#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "url.h"

url_t ori_url_r, ref_url_r;

berr url_rule_add(url_t *url_r, uint32_t id,  char *url,  char * cli_pattern, naga_acl_t *acl)
{
    if(id >= MAX_URL_RULE || url == NULL) 
    {
        return E_PARAM;
    }
    int erroffset;
    const char * error= NULL;
    
    struct pcre_s *pcre_n = &(url_r->url_pcre[id]);

    if(pcre_n->used)
    {
        pcre_n->used = 0;
        usleep(300);
        free(pcre_n->pattern);
        pcre_free(pcre_n->cre);
        
        
    }
   
    {
        pcre_n->id = id;
        pcre_n->pattern = strdup(url);
        pcre_n->cli_pattern = strdup(cli_pattern);
        pcre_n->cre =  pcre_compile( pcre_n->pattern , 0, &error, &erroffset, NULL);
		if(pcre_n->cre == NULL)
		{
			free(pcre_n->pattern);
			return E_FAIL;
		}
		
        memcpy(&pcre_n->acl, acl, sizeof(naga_acl_t));
		pcre_n->used =1;
		
    }
    if(id >= url_r->inuse)
    {
        url_r->inuse = id +1;
    }
    return E_SUCCESS;
}

struct pcre_s * url_rule_get(url_t *url_r, uint32_t id)
{
    if(id >= MAX_URL_RULE) 
    {
        return NULL;
    }    

    return &(url_r->url_pcre[id]);   
}


berr url_rule_del(url_t *url_r, uint32_t id)
{
    if(id >= MAX_URL_RULE) 
    {
        return E_PARAM;
    }
    int i, max_id;
    struct pcre_s *pcre_n = &(url_r->url_pcre[id]);

    if(pcre_n->used)
    {
        pcre_n->used = 0; 
        usleep(300);
        free(pcre_n->pattern); pcre_n->pattern = NULL;
        free(pcre_n->cli_pattern); pcre_n->cli_pattern = NULL;
        pcre_free(pcre_n->cre); pcre_n->cre = NULL;
       
    }
    
    for(i=0; i< MAX_URL_RULE; i++)
    {
    	pcre_n = &(url_r->url_pcre[i]);
        if(pcre_n->used)
            max_id = i;                                 
    }
    url_r->inuse = max_id +1;
    return E_SUCCESS;
}

#define STRING_HTTP_WB_I         "i="
#define STRING_HTTP_WB_I_LEN     2
#define STRING_HTTP_WB_S         "s="
#define STRING_HTTP_WB_S_LEN     2
#define STRING_HTTP_WB_UA        "ua="
#define STRING_HTTP_WB_UA_LEN    3
#define STRING_HTTP_WB_AID       "aid="
#define STRING_HTTP_WB_AID_LEN   4
#define STRING_HTTP_WB_GSID      "gsid="
#define STRING_HTTP_WB_GSID_LEN  5
#define STRING_HTTP_WB_UID       "uid="
#define STRING_HTTP_WB_UID_LEN   4
#define STRING_HTTP_WB_FROM      "from="
#define STRING_HTTP_WB_FROM_LEN  5

static berr pick_wb_info_from_url(hytag_t *hytag)
{
    char *token;
    char *p; 
    int len = 0;
    uint8_t flag = 0;
    if(hytag->uri[0] != '/'  || hytag->uri_len <= 0)
    {
        return E_MATCH;
    }
    token=hytag->uri;
    strsep(&token, "?");
    while(token!=NULL)
    {
        p = strsep(&token, "&");
        len = strlen(p);
        //printf("%s, len:%lu\n",p, strlen(p));
        if (hytag->wb_i_len == 0 && !strncmp(STRING_HTTP_WB_I, p, STRING_HTTP_WB_I_LEN))
        {
            if (len <= STRING_HTTP_WB_I_LEN+1 )
            {
                continue;
            }
            flag = flag | 1;
            if( len > MAX_WB_I_LEN )
            {   
                len = MAX_WB_I_LEN;
            }
            memcpy(hytag->wb_i, p, len) ; 
            hytag->wb_i_len = len; 
            continue;
        }

        if (hytag->wb_s_len == 0 && !strncmp(STRING_HTTP_WB_S, p, STRING_HTTP_WB_S_LEN))
        {
            if (len <= STRING_HTTP_WB_S_LEN+1 )
            {
                continue;
            }
            flag = flag | 2;
            if( len > MAX_WB_S_LEN )
            {   
                len = MAX_WB_S_LEN;
            }
            memcpy(hytag->wb_s, p, len) ; 
            hytag->wb_s_len = len; 
            continue;
        }

        if (hytag->wb_ua_len == 0 && !strncmp(STRING_HTTP_WB_UA, p, STRING_HTTP_WB_UA_LEN))
        {
            if (len <= STRING_HTTP_WB_UA_LEN+1 )
            {
                continue;
            }
            flag = flag | 4;
            if( len > MAX_WB_UA_LEN )
            {   
                len = MAX_WB_UA_LEN;
            }
            memcpy(hytag->wb_ua, p, len) ; 
            hytag->wb_ua_len = len; 
            continue;
        }

        if (hytag->wb_aid_len == 0 && !strncmp(STRING_HTTP_WB_AID, p, STRING_HTTP_WB_AID_LEN))
        {
            // aid content must larger 50 bytes
            if (len < STRING_HTTP_WB_AID_LEN+50 )
            {
                continue;
            }
            flag = flag | 8;
            if( len > MAX_WB_AID_LEN )
            {   
                len = MAX_WB_AID_LEN;
            }
            memcpy(hytag->wb_aid, p, len) ; 
            hytag->wb_aid_len = len; 
            continue;
        }

        if (hytag->wb_gsid_len == 0 && !strncmp(STRING_HTTP_WB_GSID, p, STRING_HTTP_WB_GSID_LEN))
        {
            if (len <= STRING_HTTP_WB_GSID_LEN+1 )
            {
                continue;
            }
            flag = flag | 16;
            if( len > MAX_WB_GSID_LEN )
            {   
                len = MAX_WB_GSID_LEN;
            }
            memcpy(hytag->wb_gsid, p, len) ; 
            hytag->wb_gsid_len = len; 
            continue;
        }

        if (hytag->wb_uid_len == 0 && !strncmp(STRING_HTTP_WB_UID, p, STRING_HTTP_WB_UID_LEN))
        {
            //uid content must be 10 bytes
            if (len != STRING_HTTP_WB_UID_LEN+10 )
            {
                continue;
            }
            flag = flag | 32;
            if( len > MAX_WB_UID_LEN )
            {   
                len = MAX_WB_UID_LEN;
            }
            memcpy(hytag->wb_uid, p, len) ; 
            hytag->wb_uid_len = len; 
            continue;
        }

        if (hytag->wb_from_len == 0 && !strncmp(STRING_HTTP_WB_FROM, p, STRING_HTTP_WB_FROM_LEN))
        {
            if( len > MAX_WB_FROM_LEN )
            {   
                len = MAX_WB_FROM_LEN;
            }
            memcpy(hytag->wb_from, p, len) ; 
            hytag->wb_from_len = len; 
            continue;
        }

    }
    if (flag == 0x3f)
    {
        return E_SUCCESS;
    }
    else
    {
        return E_MATCH;
    }

} 

berr  naga_url(url_t *url_r, hytag_t *hytag, char *url_str, int url_len)
{
    int ovector[OVECCOUNT]={0};
    struct pcre_s * urlcre= NULL;


    uint32_t i; int compare = 0;

    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        return E_SUCCESS;
    }
    CNT_INC(URL_PKTS);
    for(i=0; i<url_r->inuse; i ++ )
    {
        urlcre = &(url_r->url_pcre[i]);
        if(urlcre->used && urlcre->cre)
        {
            compare  = pcre_exec(urlcre->cre,  NULL, url_str, url_len, 0, 0, ovector, OVECCOUNT);
            if(compare > 0)
            {
                CNT_INC(URL_MATCHED);
                ACL_HIT(urlcre->acl); 

                /*
                if (compare > 1)
                {
                    memcpy(hytag->reg, (url_str + ovector[2]), (ovector[3] - ovector[2])); 
                }
                */
                HYTAG_ACL_MERGE(hytag->acl, urlcre->acl);
                if (ACT_IS_VAILD(urlcre->acl.actions, ACT_REDIR))
                {
                    if (ACL_CNT_GET(urlcre->acl)% urlcre->acl.sample < 1)
                    {
                        // push statistics
                        ACL_PUSHED_ASSERT_HIT(urlcre->acl);
                    }
                    else
                    {
                        // drop statistics
                        ACL_PRE_NOT_DROP_HIT (urlcre->acl);
                        hytag->acl.actions &= 0xFFFFFFEF;
                    }
                }

                if (ACT_IS_VAILD(urlcre->acl.actions, ACT_URLPUSH) || ACT_IS_VAILD(urlcre->acl.actions, ACT_UDPPUSH) )
                {
                   if (APP_URLPUSH_IDFA == urlcre->acl.push_type)
                   {
                       memcpy(hytag->reg, (url_str + ovector[3]+1), ovector[1]-ovector[3]-1);
                       CNT_INC(URL_URLPUSH_IDFA);
                   }
                   else
                   if (APP_URLPUSH_APPID == urlcre->acl.push_type)
                   {
                       memcpy(hytag->reg, (url_str + ovector[0]), ovector[1]-ovector[0]);
                       CNT_INC(URL_URLPUSH_APPID);
                       if (hytag->icloud_dsid_len)
                       {
                            CNT_INC(URL_URLPUSH_APPID_DSID);
                       }
                   }
                   else
                   {
                       CNT_INC(URL_URLPUSH_OTHER);
                   }
                }

                //weibo info pick up
                if ( ACT_IS_VAILD(urlcre->acl.actions, ACT_WBPUSH) )
                {
                    if (pick_wb_info_from_url(hytag) == E_SUCCESS)
                    {
                         // push statistics
                        ACL_PUSHED_ASSERT_HIT(urlcre->acl);
                    }
                    else
                    {
                        // drop statistics
                        hytag->acl.actions &= 0xFFFFFBFF;
                        ACL_PRE_NOT_DROP_HIT (urlcre->acl);
                    }
                }

                return E_SUCCESS;
            }
        }
    }
    CNT_INC(URL_DISMATCH);

	return E_SUCCESS;  

}

berr  naga_ori_url(hytag_t *hytag)
{
    return naga_url(&ori_url_r, hytag, hytag->url, hytag->url_len); 
}

berr  naga_ref_url(hytag_t *hytag)
{
    return naga_url(&ref_url_r, hytag, hytag->referer, hytag->referer_len); 
}

berr ori_url_rule_add(uint32_t id,  char *url,  char * cli_pattern, naga_acl_t *acl)
{
    return url_rule_add(&ori_url_r, id, url, cli_pattern, acl);
}

berr ref_url_rule_add(uint32_t id,  char *url,  char * cli_pattern, naga_acl_t *acl)
{
    return url_rule_add(&ref_url_r, id, url, cli_pattern, acl);
}

berr ori_url_rule_del(uint32_t id)
{
    return url_rule_del(&ori_url_r, id);
}

berr ref_url_rule_del(uint32_t id)
{
    return url_rule_del(&ref_url_r, id);
}

struct pcre_s * ori_url_rule_get(uint32_t id)
{
    return url_rule_get(&ori_url_r, id);
}

struct pcre_s * ref_url_rule_get(uint32_t id)
{
    return url_rule_get(&ref_url_r, id);
}
/* End of file */
