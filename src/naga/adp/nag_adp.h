#ifndef __NAGA_ADP_H
#define __NAGA_ADP_H

#include "boots.h"

berr naga_adp(hytag_t *hytag);

berr adp_set_interval(int interval);

berr adp_dp_init(void);

berr adp_get_interval(int *interval, uint64_t *adp_cnt, uint64_t *success);


#endif