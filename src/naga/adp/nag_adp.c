#include "naga_types.h"
#include "nag_adp.h"
#include "itf.h"
#include "packet.h"

berr naga_adp(hytag_t *hytag)
{
    
    berr rv;
    if( NULL == hytag )
    {
        return E_PARAM;
    }
    if(NULL == hytag->m)
    {
        return E_PARAM;
    }
    
    rv = ads_response_head_generator(hytag->m, hytag);
    if(rv != E_SUCCESS)
        return rv;
    
    itf_tx_burst();

    
    rv = ads_response_content_generator(hytag->m, hytag);
    if(rv != E_SUCCESS)
        return rv;
    
    itf_tx_burst();
    return E_SUCCESS;
}