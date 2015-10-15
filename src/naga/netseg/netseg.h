/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/mnt/hgfs/code/rose/src/dpdk-2.0.0/examples/zebra/vsr\vsr.h
#       @date         :2015/08/21 22:56
#       @algorithm    :
=============================================================================*/
#include "stdio.h"
#include "string.h"
#include "boots.h"
#include "rte_atomic.h"
#include "rte_spinlock.h"
#include "naga_types.h"

#ifndef  __NETSEG_H__
#define  __NETSEG_H__

#define NETSEG_RULE_NUM_MAX  100
#define NETSEG_RULE_EFFECTIVE   1
#define NETSEG_RULE_UNEFFECTIVE 0

typedef struct {
    uint32_t index;
	uint32_t ip;
	uint32_t mask;
    naga_acl_t acl;
} net_t;

typedef rte_spinlock_t  net_lock_t;

typedef struct {
    net_t net;
    net_lock_t lock;
    uint8_t effective;
} netseg_t;


berr api_net_add(net_t *net);
berr api_net_del(uint32_t index);
berr  api_net_get(uint32_t index, net_t *net);
berr api_net_clear_statistics(uint32_t index);

/* dp use */
berr api_net_dp_match(uint32_t index, uint32_t ip);
berr netseg_init(void);

#endif /* end of __NET_H__ */
