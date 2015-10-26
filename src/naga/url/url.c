#include <stdio.h>
#include <string.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "url.h"
#include "pcre.h"




url_t url_r;

berr url_rule_add(uint32_t id, const char *url, uint32_t action)
{
    if(id >= MAX_URL_RULE || url == NULL) 
    {
        return E_PARAM;
    }
    int erroffset;
    const char * error= NULL;
    
    struct pcre_s *pcre_n = &(url_r.url_pcre[id]);

    if(pcre_n->used)
    {
        free(pcre_n->pattern);
        pcre_free(pcre_n->cre);
        
        
    }
   
    {
        pcre_n->id = id;
        pcre_n->pattern = strdup(url);
        pcre_n->cre =  pcre_compile( pcre_n->pattern , 0, &error, &erroffset, NULL);
        pcre_n->acl.actions = action;
    }
    if(id > url_r.inuse)
    {
        url_r.inuse = id;
    }
    return E_SUCCESS;
}

struct pcre_s * url_rule_get(uint32_t id)
{
    if(id >= MAX_URL_RULE) 
    {
        return NULL;
    }    

    return &(url_r.url_pcre[id]);   
}


berr url_rule_del(uint32_t id)
{
    if(id >= MAX_URL_RULE) 
    {
        return E_PARAM;
    }
    int i, max_id;
    struct pcre_s *pcre_n = &(url_r.url_pcre[id]);

    if(pcre_n->used)
    {
        free(pcre_n->pattern);
        pcre_free(pcre_n->cre);
        pcre_n->used = 0;     
    }
    

   
    for(i=0; i< MAX_URL_RULE; i++)
    {
        if(pcre_n->used)
            max_id = i;                                 
    }
    url_r.inuse = max_id;
    return E_SUCCESS;
}


berr  naga_uri(hytag_t *hytag)
{

/* if(!strcmp((char *)hytag->host, "180.96.27.113"))
    {
        CNT_INC(ADP_DROP_NOT_HAO123);	         
    }
  */
    //char str_tail[10] ={'\0'};

#define OVECCOUNT 30

    const char *tail = "/?_tTI=tTI";
    char  *tailptr = NULL;
    int ovector[OVECCOUNT];
    struct pcre_s * urlcre= NULL;
    //if(hytag->uri_len >= 8)    
    //    tailptr = (char *)(&(hytag->uri[hytag->uri_len - 4]));//the last 
    //else
    tailptr = (char *)(hytag->uri);
    
    uint32_t i; int compare;
    //if(strstr(tailptr,  tail))
    if(!strcmp(tailptr, tail))

    {
         CNT_INC(ADP_PUSH_ACK_SUCCESS);
         hytag->pushed_second_assert = 1;            
    }
    
    if(hytag->uri_len == 1 && !strcmp(hytag->uri, "/"))
    {    
    	hytag->acl.actions |=  ACT_LOG;
        CNT_INC(URL_HOMEPAGE);
		return E_SUCCESS;
    }
    else 
    {
    	//hytag->acl.actions |=  ACT_DROP;
        //CNT_INC(ADP_DROP_BACKSLASH_SUFFIX);
        //return E_SUCCESS;  
        for(i=0; i<url_r.inuse; i ++ )
        {
            urlcre = &(url_r.url_pcre[i]);
            if(urlcre->used && urlcre->cre)
            {
                compare  = pcre_exec(urlcre->cre,
                                NULL, hytag->uri, hytag->uri_len, 0, 0, ovector, OVECCOUNT);

                if(compare > 0)
                {
                   ACL_HIT(urlcre->acl);
                   HYTAG_ACL_MERGE(hytag->acl, urlcre->acl);
                   return E_SUCCESS;
                }
                

            }
        }  
    }

	hytag->acl.actions |=  ACT_DROP;
	CNT_INC(ADP_DROP_BACKSLASH_SUFFIX);
	return E_SUCCESS;  

}


