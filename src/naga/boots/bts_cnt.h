#ifndef __BTS_CNT_H__
#define __BTS_CNT_H__

#include "boots.h"

typedef enum {
      ITF_IPKTS,
      ITF_IBYTS,
      ITF_OPKTS,
      ITF_OBYTS,
#ifdef CNT_CUSTOM
      CNT_CUSTOM,
#endif
      CNT_MAX
} cnte;
 
typedef struct {
      cnte idx;
      char *name;
      bts_atomic64_t val;
} cnt_t;


#define CNT_DEF(_cnt) {_cnt, #_cnt}
// ??????
#define CNT_INC(_cnt)       bts_atomic64_inc(&(cnt_array[_cnt].val))
#define CNT_DEC(_cnt)       bts_atomic64_dec(&(cnt_array[_cnt].val))
#define CNT_ADD(_cnt, _val) bts_atomic64_add(&(cnt_array[_cnt].val), _val)
#define CNT_SET(_cnt, _val) bts_atomic64_set(&(cnt_array[_cnt].val), _val)
#define CNT_GET(_cnt)       bts_atomic64_get(&(cnt_array[_cnt].val))

// ????????????????
berr cnt_add(cnte idx, uint64_t value);
berr cnt_inc(cnte idx);
berr cnt_dec(cnte idx);
berr cnt_get(cnte idx, uint32_t number, cnt_t *vals, uint32_t *total);
berr cnt_clear(cnte idx, uint32_t number, uint32_t *total);
berr cnt_int();

#endif
