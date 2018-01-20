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

    rv = naga_ori_url(hytag);

    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR)) {
        return rv;
    }

    rv = naga_ref_url(hytag);

    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR)) {
        return rv;
    }

    rv = naga_mask(hytag);

    if (E_SUCCESS == rv && (hytag->acl.actions & ACT_REDIR)) {
        return rv;
    }

    return E_SUCCESS;
}

/* End of file */
