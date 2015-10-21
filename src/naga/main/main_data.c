
#include <rte_config.h>
#include <rte_common.h>
#include <rte_log.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_atomic.h>
#include <rte_cycles.h>
#include <rte_prefetch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_branch_prediction.h>
#include <rte_interrupts.h>
#include <rte_pci.h>
#include <rte_random.h>
#include <rte_debug.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_ring.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>

#include "boots_custom.h"
#include "dbg_tagmon.h"
#include <string.h>
#include "vsr_dp.h"
#include "itf.h"
#include "nag_adp.h"
#include "bts_cnt.h"


#define MOD_IS_TURN_ON(mod)  (mod_is_enable(mod) == ON)


#define DPF_NODE(_mod, _tag, _func) { \
    if(MOD_IS_TURN_ON(_mod)) \
    { \
        berr _ec = _func(_tag); \
        if (_ec == E_SUCCESS) \
        { \
            /*DBG_INFO(MOD_DPF, "%s ...... PASS.\n", #_func);*/ \
        } else { \
            DBG_ERR(MOD_DPF, "%s  ...... FAIL!(%s)\n", #_func, berr_msg(_ec)); \
        } \
        HYTAG_DUMP(_tag); \
    } \
}


/*
 * data plane main process flow, not include rx & tx
*/
int print_t = 0;
berr naga_data_process_module(hytag_t * hytag)
{

	if (!itf_rx_is_enable())
	{
		return E_SUCCESS;
	}

    itf_set_hytag_pcap(hytag);

    struct timeval start, end; 
    
    //HYTAG_DUMP(hytag);
    gettimeofday(&start,NULL);
    DPF_NODE(MOD_PID, hytag, naga_pid);
	gettimeofday(&end,NULL);

    uint64_t times;

    if(print_t < 200)
    {
      times = (end.tv_sec * 1000*1000 + end.tv_usec)
                - (start.tv_sec * 1000*1000 + start.tv_usec);
        printf("%d-PID: %lu\n" , print_t, times);
    }

    gettimeofday(&start,NULL);
	DPF_NODE(MOD_ADP, hytag, naga_uri);
	gettimeofday(&end,NULL);
    
    if(print_t < 200)
    {
         times = (end.tv_sec * 1000*1000 + end.tv_usec)
        - (start.tv_sec * 1000*1000 + start.tv_usec);
        printf("%d-URL: %lu\n" , print_t, times);
    }

    gettimeofday(&start,NULL);
	DPF_NODE(MOD_NET, hytag, netseg_dp_process);
	gettimeofday(&end,NULL);
    
    if(print_t < 200)
    {
         times = (end.tv_sec * 1000*1000 + end.tv_usec)
        - (start.tv_sec * 1000*1000 + start.tv_usec);
        
        printf("%d-SNET: %lu\n" , print_t, times);
    }

    gettimeofday(&start,NULL);
	DPF_NODE(MOD_NET, hytag, dnet_dp_process);
	gettimeofday(&end,NULL);
    
    if(print_t < 200)
    {
        times = (end.tv_sec * 1000*1000 + end.tv_usec)
        - (start.tv_sec * 1000*1000 + start.tv_usec);        
        printf("%d-DNET: %lu\n" ,print_t, times);
    }

    gettimeofday(&start,NULL);
    //DPF_NODE(MOD_VSR, &hytag, naga_vsr);
    DPF_NODE(MOD_DMR, hytag, naga_dmr);
	gettimeofday(&end,NULL);
    if(print_t < 200)
    {
        times = (end.tv_sec * 1000*1000 + end.tv_usec)
        - (start.tv_sec * 1000*1000 + start.tv_usec);     
        printf("%d-DMR: %lu\n" , print_t,  times);
    }

    gettimeofday(&start,NULL);
    DPF_NODE(MOD_ACR, hytag, naga_acr);
	gettimeofday(&end,NULL);
    if(print_t < 200)
    {
        times = (end.tv_sec * 1000*1000 + end.tv_usec)
        - (start.tv_sec * 1000*1000 + start.tv_usec);          
        printf("%d-DMR: %lu\n" ,print_t,  times);
    }
    
    gettimeofday(&start,NULL);
    DPF_NODE(MOD_ADP, hytag, naga_adp);
    gettimeofday(&end,NULL);

    if(hytag->ad_act == AD_SUCCESS)
    {
        times = (end.tv_sec * 1000*1000 + end.tv_usec)
        - (start.tv_sec * 1000*1000 + start.tv_usec);  
        
        printf("%d-ADP: %lu\n" ,print_t ,  times);
    }
    print_t++;
    
    HYTAG_LOG(hytag);  
    return E_SUCCESS;
}

berr naga_data_process_flow(struct rte_mbuf *m)
{
    hytag_t hytag;

    memset(&hytag, 0x0, sizeof(hytag));

  
    hytag.pbuf.ptr = rte_pktmbuf_mtod(m, void *);
    hytag.pbuf.len = m->data_len;
    hytag.pbuf.ptr_offset = 0;
    hytag.m = m;
    

    return naga_data_process_module(&hytag);
}


#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */

void naga_data_main_loop()
{
	uint64_t prev_tsc = 0, diff_tsc = 0, cur_tsc = 0;
	const uint64_t drain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US;

    if(itf_port_check() != E_SUCCESS)
	{
        return;
    }

	while (1)
	{
        #if 0
		cur_tsc = rte_rdtsc();

		/*
		 * TX burst queue drain
		 */
		 
		diff_tsc = cur_tsc - prev_tsc;
		if (unlikely(diff_tsc > drain_tsc)) {
			itf_tx_burst();

			prev_tsc = cur_tsc;
		}
        #endif
		/*
		 * Read packet from RX queues
		 */

        itf_rx_burst(naga_data_process_flow);

	}
}

/* End of file */
