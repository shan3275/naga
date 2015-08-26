#ifndef __BTS_CNT_H__
#define __BTS_CNT_H__

#include "boots.h"

typedef enum {
      ITF_IPKTS,
      ITF_IBYTS,
      ITF_OPKTS,
      ITF_OBYTS,
#ifdef CNT_CUSTOM
      CNT_CUSTOM
#endif
      CNT_MAX
} cnte;
 
typedef struct {
      cnte idx;
      char *name;
      uint64_t val;
} cnt_t;

#define CNT_DEC(_cnt)  {_cnt, #_cnt}

 
// ??????
#define CNT_INC(_cnt)
#define CNT_ADD(_cnt, _val)
 
// ????????????????
berr cnt_add(cnte idx, uint64_t value);
berr cnt_get(cnte idx, uint32_t number, cnt_t *cnts, uint32_t *sum);
berr cnt_clear(cnte idx, uint32_t number, uint32_t *sum);
berr cnt_int(void);

#endif
