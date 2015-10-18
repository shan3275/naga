#include <stdio.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "url.h"




berr  naga_uri(hytag_t *hytag)
{
	
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


