#include "itf.h"
#include "bts_debug.h"

extern struct itf_port_statistics port_statistics[RTE_MAX_ETHPORTS];

extern struct lcore_queue_conf lcore_queue_conf[RTE_MAX_LCORE];

berr itf_port_check(void)
{
    uint32_t i, portid = 0;
    unsigned int lcore_id;
    struct lcore_queue_conf *qconf = NULL;

    lcore_id = rte_lcore_id();
    qconf = &lcore_queue_conf[lcore_id];

    if (qconf->n_rx_port == 0) {
        DBG_INFO(MOD_ITF,"lcore %u has nothing to do\n", lcore_id);
        BRET(E_INIT);
    }

    DBG_INFO(MOD_DBG,"entering main loop on lcore %u\n", lcore_id);

    for (i = 0; i < qconf->n_rx_port; i++) {
        portid = qconf->rx_port_list[i];
        DBG_INFO(MOD_ITF," -- lcoreid=%u portid=%u\n", lcore_id, portid);
    }

    BRET(E_SUCCESS);
}

void itf_rx_burst(rx_process_func process_func)
{
    struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
	struct rte_mbuf *m;
    unsigned int lcore_id;
    struct lcore_queue_conf *qconf = NULL;

    unsigned i, j, portid, nb_rx;

    berr ec = E_SUCCESS;

    lcore_id = rte_lcore_id();
    qconf = &lcore_queue_conf[lcore_id];

    for (i = 0; i < qconf->n_rx_port; i++) {

        portid = qconf->rx_port_list[i];
        nb_rx = rte_eth_rx_burst((uint8_t) portid, 0,
                     pkts_burst, MAX_PKT_BURST);

        port_statistics[portid].rx += nb_rx;

        for (j = 0; j < nb_rx; j++) {
            m = pkts_burst[j];
            rte_prefetch0(rte_pktmbuf_mtod(m, void *));

            ec = process_func(m);
            if(ec != E_SUCCESS) {
                port_statistics[portid].fail += 1;
                DBG_WARN(MOD_ITF, "Packet process fail:%s!\n", berr_msg(ec));

                //DOPT_SELECT(DOPT_FPR, );
            }

            rte_pktmbuf_free(m);
        }
    }
}

/* End of file */
