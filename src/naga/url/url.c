#include <stdio.h>
#include <string.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "url.h"
#include "pcre.h"

#define MAX_URL_RULE   32


struct pcre_s
{
    int id;
    int used;
    char *pattern;    
    pcre *cre;
};


typedef struct
{
    int inuse;
    struct pcre_s url_pcre[MAX_URL_RULE] ;  
};


int url_rule_add(int id, char *url)
{
                
}



berr  naga_uri(hytag_t *hytag)
{

/*    if(!strcmp((char *)hytag->host, "180.96.27.113"))
    {
        CNT_INC(ADP_DROP_NOT_HAO123);	         
    }
  */
    //char str_tail[10] ={'\0'};
    const char *tail = "/?_tTI=tTI";
    char  *tailptr = NULL;


    //if(hytag->uri_len >= 8)    
    //    tailptr = (char *)(&(hytag->uri[hytag->uri_len - 4]));//the last 
    //else
    tailptr = (char *)(hytag->uri);
    
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
    }
    else
    {
    	hytag->acl.actions |=  ACT_DROP;
        CNT_INC(ADP_DROP_BACKSLASH_SUFFIX);
        return E_SUCCESS;           
    }
    
	return E_SUCCESS;
}


