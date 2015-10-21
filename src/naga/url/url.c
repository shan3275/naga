#include <stdio.h>
#include <string.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "url.h"




berr  naga_uri(hytag_t *hytag)
{

/*    if(!strcmp((char *)hytag->host, "180.96.27.113"))
    {
        CNT_INC(ADP_DROP_NOT_HAO123);	         
    }
  */
    //char str_tail[10] ={'\0'};
    const char *tail = "_t=t";
    char *tailptr = NULL;


    if(hytag->uri_len >= 8)  
    {
        tailptr = (char *)(&(hytag->uri[hytag->uri_len - 7]));//the last 
    }   
    else
    {
        tailptr = (char *)(hytag->uri);
    }


    if(strstr(tailptr,  tail) != NULL)
    //if(!strcmp(tailptr, tail))
    {
         CNT_INC(ADP_PUSH_ASSERT);
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


