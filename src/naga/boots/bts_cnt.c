#include "bts_cnt.h"
#include "bts_debug.h"


cnt_t cnt_array[CNT_MAX] =
{
    CNT_DEF(ITF_IPKTS),
    CNT_DEF(ITF_IBYTS),
    CNT_DEF(ITF_OPKTS),
    CNT_DEF(ITF_OBYTS),
#ifdef CNT_DEF_CUSTOM
    CNT_DEF_CUSTOM,
#endif
};

berr cnt_add(cnte idx, uint64_t value)
{
    if (idx >= CNT_MAX) 
    {
        BRET(E_EXCEED);
    }

    CNT_ADD(idx, value);

    BRET(E_SUCCESS);
}

berr cnt_inc(cnte idx)
{
    if (idx >= CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    CNT_INC(idx);

    BRET(E_SUCCESS);
}

berr cnt_dec(cnte idx)
{
    if (idx >= CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    CNT_DEC(idx); 

    BRET(E_SUCCESS);
}

berr cnt_get(cnte idx, uint32_t number, cnt_t *vals, uint32_t *total)
{
    uint32_t i  = 0, idxc = 0;

    if ((idx + number) > CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    if ((NULL == vals) || (NULL == total))
    {
        BRET(E_PARAM);
    }

    *total = 0;

    for (i = 0; i < number; i++)
    {
        idxc = idx + i;
        vals[i] = cnt_array[idxc];
        *total += 1;
    }

    BRET(E_SUCCESS);
}

berr cnt_clear(cnte idx, uint32_t number, uint32_t *total)
{
    uint32_t i  = 0, idxc = 0;

    if ((idx + number) > CNT_MAX)
    {
        BRET(E_EXCEED);
    }

    if (NULL == total)
    {
        BRET(E_PARAM);
    }

    *total = 0;

    for (i = 0; i < number; i++)
    {
        idxc = idx + i;
        CNT_SET(idxc, 0); 
        *total += 1;
    }

    BRET(E_SUCCESS);
}

berr cnt_int()
{
    uint32_t i = 0;

    for (i = 0; i < CNT_MAX; i++)
    {
        CNT_SET(i, 0);
    }

    BRET(E_SUCCESS);
}

/* End of file */
