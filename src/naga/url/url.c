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


berr  naga_url(url_t *url_r, hytag_t *hytag, char *url_str, int url_len)
{
    int ovector[OVECCOUNT];
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
                
                if (ACT_IS_VAILD(urlcre->acl.actions, ACT_REDIR))
                {
                    if (ACL_CNT_GET(urlcre->acl)% urlcre->acl.sample < 1)
                    {
                        if (compare > 1)
                        {
                            memcpy(hytag->reg, (url_str + ovector[2]), (ovector[3] - ovector[2])); 
                        }
                        // push statistics
                        ACL_PUSHED_ASSERT_HIT(urlcre->acl);
                        HYTAG_ACL_MERGE(hytag->acl, urlcre->acl);
                    }
                    else
                    {
                        // drop statistics
                        ACL_PRE_NOT_DROP_HIT (urlcre->acl);
                    }
            
                }
                else
                {
                    if (compare > 1)
                    {
                        memcpy(hytag->reg, (url_str + ovector[2]), (ovector[3] - ovector[2])); 
                    }
                    HYTAG_ACL_MERGE(hytag->acl, urlcre->acl);
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
    //char url_str[URL_MAX_LEN + 1];
    //int url_len = 0;
    
    //memset(url_str, 0, URL_MAX_LEN + 1);
    //url_len = snprintf(url_str, URL_MAX_LEN, "%s%s", hytag->ori_url.host, hytag->ori_url.uri); 

    return naga_url(&ori_url_r, hytag, hytag->ori_url.url, strlen(hytag->ori_url.url)); 
}

berr  naga_ref_url(hytag_t *hytag)
{
    //char url_str[URL_MAX_LEN + 1];
    //int url_len = 0;
    
    //memset(url_str, 0, URL_MAX_LEN + 1);
   // url_len = snprintf(url_str, URL_MAX_LEN, "%s%s", hytag->ref_url.host, hytag->ref_url.uri); 

    return naga_url(&ref_url_r, hytag, hytag->ref_url.url, strlen(hytag->ref_url.url));
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
