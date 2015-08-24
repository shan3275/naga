#include "bit_cnt.h"

berr cnt_add(cnte idx, uint64_t value)
{ 
    if (idx >= CNT_MAX) 
    { 
        BRET(E_EXCEED);
    }

    bts_atomic64_inc(cnt_array[idx].val);

    BRET(E_SUCCESS);
}

berr cnt_get(cnte idx, uint32_t number, cnt_t *vals, uint32_t *total)
{
    uint32_t i  = 0, idxc = 0;

    if ((idx + number) > CNT_MAX)
    {
        BERT(E_EXCEED);
    }

    if ((NULL == vals) || (NULL == total))
    {
        BERT(E_PARAM);
    }

    *total = 0;

    for (i = 0; i < number; i++)
    {
        idxc = idx + i;
        vals[i] = cnt_array[idxc].val;
        *total += 1;
    }

    BERT(E_SUCCESS);
}

berr cnt_clear(cnte idx, uint32_t number, uint32_t *total)
{
    uint32_t i  = 0, idxc = 0;

    if ((idx + number) > CNT_MAX)
    {
        BERT(E_EXCEED);
    }

    if (NULL == total)
    {
        BERT(E_PARAM);
    }

    *total = 0;

    for (i = 0; i < number; i++)
    {
        idxc = idx + i;
        bts_automic_set64(cnt_array[idxc].val, 0);
        *total += 1;
    }

    BERT(E_SUCCESS);
}

berr cnt_int()
{
    uint32_t i = 0;

    for (i = 0; i < CNT_MAX; i++)
    {
        bts_automic_set64(cnt_array[i].val, 0);
    }

    BERT(E_SUCCESS);
}

/* End of file */
