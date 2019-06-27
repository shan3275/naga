/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2016 Intel Corporation
 */

#ifndef __L3_FWD_H__
#define __L3_FWD_H__

#include <rte_vect.h>

#define DO_RFC_1812_CHECKS

#define RTE_LOGTYPE_L3FWD RTE_LOGTYPE_USER1

#if !defined(NO_HASH_MULTI_LOOKUP) && defined(RTE_MACHINE_CPUFLAG_NEON)
#define NO_HASH_MULTI_LOOKUP 1
#endif

#define MAX_PKT_BURST     32
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */

#define MAX_RX_QUEUE_PER_LCORE 16

/*
 * Try to avoid TX buffering if we have at least MAX_TX_BURST packets to send.
 */
#define	MAX_TX_BURST	  (MAX_PKT_BURST / 2)

#define NB_SOCKETS        8

/* Configure how many packets ahead to prefetch, when reading packets */
#define PREFETCH_OFFSET	  3

/* Used to mark destination port as 'invalid'. */
#define	BAD_PORT ((uint16_t)-1)

#define FWDSTEP	4

/* replace first 12B of the ethernet header. */
#define	MASK_ETH 0x3f

/* Hash parameters. */
#ifdef RTE_ARCH_64
/* default to 4 million hash entries (approx) */
#define L3FWD_HASH_ENTRIES		(1024*1024*4)
#else
/* 32-bit has less address-space for hugepage memory, limit to 1M entries */
#define L3FWD_HASH_ENTRIES		(1024*1024*1)
#endif
#define HASH_ENTRY_NUMBER_DEFAULT	4

struct mbuf_table {
	uint16_t len;
	struct rte_mbuf *m_table[MAX_PKT_BURST];
};

struct lcore_rx_queue {
	uint16_t port_id;
	uint8_t queue_id;
} __rte_cache_aligned;

struct lcore_conf {
	uint16_t n_rx_queue;
	struct lcore_rx_queue rx_queue_list[MAX_RX_QUEUE_PER_LCORE];
	uint16_t n_tx_port;
	uint16_t tx_port_id[RTE_MAX_ETHPORTS];
	uint16_t tx_queue_id[RTE_MAX_ETHPORTS];
	struct mbuf_table tx_mbufs[RTE_MAX_ETHPORTS];
	void *ipv4_lookup_struct;
	void *ipv6_lookup_struct;
} __rte_cache_aligned;

extern volatile bool force_quit;

/* ethernet addresses of ports */
extern uint64_t dest_eth_addr[RTE_MAX_ETHPORTS];
extern struct ether_addr ports_eth_addr[RTE_MAX_ETHPORTS];

/* mask of enabled ports */
extern uint32_t enabled_port_mask;

/* Used only in exact match mode. */
extern int ipv6; /**< ipv6 is false by default. */
extern uint32_t hash_entry_number;

extern xmm_t val_eth[RTE_MAX_ETHPORTS];

extern struct lcore_conf lcore_conf[RTE_MAX_LCORE];



/* Function pointers for LPM or EM functionality. */
void
setup_lpm(const int socketid);

int
lpm_check_ptype(int portid);

uint16_t
lpm_cb_parse_ptype(uint16_t port, uint16_t queue, struct rte_mbuf *pkts[],
		   uint16_t nb_pkts, uint16_t max_pkts, void *user_param);


int
l3fwd_launch_one_lcore(__attribute__((unused)) void *dummy);

/* Return ipv4/ipv6 fwd lookup struct for LPM or EM. */

void *
lpm_get_ipv4_l3fwd_lookup_struct(const int socketid);

void *
lpm_get_ipv6_l3fwd_lookup_struct(const int socketid);

#endif  /* __L3_FWD_H__ */
