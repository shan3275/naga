#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"

#include "naga_types.h"
#include "naga_fct.h"
#include "url.h"
#include "mask.h"

berr naga_fct(hytag_t *hytag)
{
    berr rv = E_MAX;

    if( APP_TYPE_HTTP_GET_OR_POST != hytag->app_type)
    {
        return E_SUCCESS;
    }

    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }

    rv = netseg_dp_process(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }

    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }

    rv = naga_dmr(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }

    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }

    /*
    rv = naga_urlr(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }
    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }
    */

    rv = naga_ori_url(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }
    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }

    rv = naga_ref_url(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }

    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }
    rv = naga_ua(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }

    if(ACT_IS_VAILD(hytag->acl.actions, ACT_DROP))
    {
        return E_SUCCESS;
    }

    rv = naga_mask(hytag);
    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR ||hytag->acl.actions & ACT_URLPUSH ||hytag->acl.actions & ACT_UDPPUSH))
    {
        return rv;
    }

    return E_SUCCESS;
}

/* End of file */
