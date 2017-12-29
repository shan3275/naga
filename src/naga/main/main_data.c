
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
//#include <rte_mbuf.h>

#include "boots_custom.h"
#include "dbg_tagmon.h"
#include <string.h>
#include "vsr_dp.h"
#include "itf.h"
#include "nag_adp.h"
#include "naga_hijack.h"
#include "bts_cnt.h"
#include "main_data.h"
#include "domain_dp.h"


#define MOD_IS_TURN_ON(mod)  (mod_is_enable(mod) == ON)


#define DPF_NODE(_mod, _tag, _func) { \
    if(MOD_IS_TURN_ON(_mod)) \
    { \
        berr _ec = _func(_tag); \
        if (_ec == E_SUCCESS) \
        { \
            /*DBG_INFO(MOD_DPF, "%s ...... PASS.\n", #_func);*/ \
        } else { \
           /* DBG_ERR(MOD_DPF, "%s  ...... FAIL!(%s)\n", #_func, berr_msg(_ec));*/ \
        } \
        HYTAG_DUMP(_tag); \
    } \
}


pthread_mutex_t naga_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * data plane main process flow, not include rx & tx
*/
#define CHECK_PACK_TIME 0
berr naga_data_process_module(hytag_t * hytag)
{

	if (!itf_rx_is_enable())
	{
		return E_SUCCESS;
	}

#if CHECK_PACK_TIME
	struct timeval tv0, tv1; 
	gettimeofday(&tv0, NULL);	
#endif

    itf_set_hytag_pcap(hytag);

    //HYTAG_DUMP(hytag);

    DPF_NODE(MOD_PID, hytag, naga_pid);
    DPF_NODE(MOD_ADP, hytag, naga_uri);
    DPF_NODE(MOD_ADP, hytag, naga_referer);


    //DPF_NODE(MOD_NET, hytag, netseg_dp_process);
    //DPF_NODE(MOD_DNET, hytag, dnet_dp_process);


    //DPF_NODE(MOD_DMR, hytag, naga_dmr);

    //DPF_NODE(MOD_DMR, hytag, naga_domain);

    //DPF_NODE(MOD_ACR, hytag, naga_acr);

    //DPF_NODE(MOD_ADP, hytag, naga_adp);

    //DPF_NODE(MOD_HIJACK, hytag, naga_hijack);
   
    pthread_mutex_lock(&naga_mutex);
    HYTAG_LOG(hytag);
    pthread_mutex_unlock(&naga_mutex);
#if CHECK_PACK_TIME	
	gettimeofday(&tv1, NULL); 

	uint64_t m_time = 0;
	uint64_t s_time = 0;
	if(hytag->ad_act == AD_SUCCESS)
	{
		m_time = tv0.tv_sec;
		m_time = m_time*1000*1000;
		m_time += tv0.tv_usec;

		s_time = tv1.tv_sec;
		s_time = s_time*1000*1000;
		s_time += tv1.tv_usec;
		
		printf("Take time = %ld us\n", s_time- m_time);	
	}
#endif	
    return E_SUCCESS;
}
