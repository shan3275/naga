#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "ua.h"

ua_t ua_r;

berr  naga_ua(hytag_t *hytag)
{
    ua_t *url_r = &ua_r;
    char *url_str = hytag->user_agent;
    int url_len = hytag->user_agent_len;
    int ovector[OVECCOUNT]={0};
    struct pcre_s * urlcre= NULL;

    uint32_t i; int compare = 0;

    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        return E_SUCCESS;
    }

    CNT_INC(UA_PKTS);
    for(i=0; i<url_r->inuse; i ++ )
    {
        urlcre = &(url_r->ua_pcre[i]);
        if(urlcre->used && urlcre->cre)
        {
            compare  = pcre_exec(urlcre->cre,  NULL, url_str, url_len, 0, 0, ovector, OVECCOUNT);
            if(compare > 0)
            {
                CNT_INC(UA_MATCHED);
                ACL_HIT(urlcre->acl); 

                if (compare > 1)
                {
                    memcpy(hytag->reg, (url_str + ovector[2]), (ovector[3] - ovector[2])); 
                }
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

                return E_SUCCESS;
            }
        }
    }
    CNT_INC(UA_DISMATCH);

    return E_SUCCESS;  
}

berr ua_rule_add(uint32_t id,  char *url,  char * cli_pattern, naga_acl_t *acl)
{
    ua_t *url_r = &ua_r;
    if(id >= MAX_UA_RULE || url == NULL) 
    {
        return E_PARAM;
    }
    int erroffset;
    const char * error= NULL;
    
    struct pcre_s *pcre_n = &(url_r->ua_pcre[id]);

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

berr ua_rule_del(uint32_t id)
{
    ua_t *url_r = &ua_r;
    if(id >= MAX_UA_RULE) 
    {
        return E_PARAM;
    }
    int i, max_id;
    struct pcre_s *pcre_n = &(url_r->ua_pcre[id]);

    if(pcre_n->used)
    {
        pcre_n->used = 0; 
        usleep(300);
        free(pcre_n->pattern); pcre_n->pattern = NULL;
        free(pcre_n->cli_pattern); pcre_n->cli_pattern = NULL;
        pcre_free(pcre_n->cre); pcre_n->cre = NULL;
       
    }
    
    for(i=0; i< MAX_UA_RULE; i++)
    {
    	pcre_n = &(url_r->ua_pcre[i]);
        if(pcre_n->used)
            max_id = i;                                 
    }
    url_r->inuse = max_id +1;
    return E_SUCCESS;
}

struct pcre_s * ua_rule_get(uint32_t id)
{
    ua_t *url_r = &ua_r;
    if(id >= MAX_UA_RULE) 
    {
        return NULL;
    }    

    return &(url_r->ua_pcre[id]);   
}
/* End of file */
