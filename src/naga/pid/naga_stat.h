#ifndef _NAGA_STAT_H
#define _NAGA_STAT_H

#include "rte_atomic.h"
#include "bts_cnt.h"
typedef rte_atomic64_t pid_atomic64_cnt_t ;

struct pid_port_stat_s
{
    pid_atomic64_cnt_t  stat;
}__rte_cache_aligned;


//void pid_incr_count(enum pid_pkt_type pid_type) ;
void gpid_stats_init(void);

void printf_pid_stat(void);



#define pid_incr_count cnt_inc
#define pid_add_count  cnt_add

#endif
