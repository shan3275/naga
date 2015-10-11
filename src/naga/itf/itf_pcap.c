#include <stdio.h>
#include "netinet/in.h"
#include <pcap.h>
#include <pthread.h>
#include "boots.h"
#include "bts_debug.h"

#include "itf.h"
#include "boots_custom.h"
#include "dbg_tagmon.h"
#include "itf_stat.h"
#include "bts_cnt.h"
#include "bts_list.h"


pcap_t *gpcap_desc = NULL;

berr itf_raw_socket_init(char *ifname)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    if(ifname== NULL)
        return E_SUCCESS;

    
    /* Open the adapter */
    if ((gpcap_desc = pcap_open_live(ifname,  // name of the device
               65536,                 // portion of the packet to capture. It doesn't matter in this case
                 1|                   // promiscuous mode (nonzero means promiscuous)
                 8 |
                 16,
                 0,                     // read timeout
                 errbuf                 // error buffer
        )) == NULL) 
    {
            printf( "Pcap unable open the adapter %s reason is:\n", ifname);
            printf( "%s\n", errbuf );
            BRET(E_FAIL);
    }
    return E_SUCCESS;
}


berr ift_raw_send_packet(void* fp, uint8_t * buff, int len)
{
    int rv;
    if (itf_tx_is_enable())
    {
        rv =  pcap_sendpacket((pcap_t *)fp, buff, len); 
            
            if(rv < 0)
            {
                printf("Send Packet Failed %s %d\n", __func__, __LINE__);
                    BRET(E_FAIL);
            }		
        	cnt_inc(ITF_OPKTS);
        	cnt_add(ITF_OBYTS, len);
    }
    return E_SUCCESS;
}


void itf_set_hytag_pcap(hytag_t * tag)
{
    if(gpcap_desc != NULL)
    {
        tag->fp = gpcap_desc;
        tag->eth_tx = ENABLE;
    }        
    return ;
}

typedef struct
{
    pcap_t *fp;
}libpcap_param_t;

void libpcap_packet_handler(u_char *param __attribute__((unused)), 
                            const struct pcap_pkthdr *header,  u_char *packet);
extern berr naga_data_process_module(hytag_t * tag);

void libpcap_packet_handler(u_char *param __attribute__((unused)), 
                            const struct pcap_pkthdr *header,   u_char *packet)
{
    hytag_t hytag;
    //char buffer[2048];	
	
    memset(&hytag, 0x0, sizeof(hytag));
    //memcpy((void *)buffer, (void *)packet, header->len);	
    
    hytag.pbuf.ptr = (void *)packet;
    hytag.pbuf.len = header->len;
    hytag.pbuf.ptr_offset = 0;
    hytag.m = NULL;
    //printf("Success packet len = %d\n", hytag.pbuf.len);
    //return;

	cnt_inc(ITF_IPKTS);
	cnt_add(ITF_IBYTS, header->len);

	
    naga_data_process_module(&hytag);
    return;
}

void* pcap_rx_loop(void *_param);
void *pcap_rx_loop(void *_param)
{
    libpcap_param_t *param = (libpcap_param_t *)_param;
	
    pcap_loop(param->fp, 0, (pcap_handler)libpcap_packet_handler, (void*)_param);
    return NULL;
}






#define NAGA_CONTROL_FILTER "dst port 80"

typedef struct
{
	pthread_t recv_thread;
	pcap_t * fp;
	char   *ifname;
	struct list_head node;
}libpcap_handler_t;



BTS_LIST_HEAD(handle_head);


berr libpcap_rx_loop_setup(char * ifname)
{
        pcap_t *fp = NULL;
        char packet_filter[128];
        struct bpf_program fcode;
        libpcap_param_t param ;
		pthread_t recv_thread;

		libpcap_handler_t *pos = NULL, *next = NULL;
		
		list_for_each_entry_safe(pos, next, (&handle_head), node)

		{			
			if(!strcmp(pos->ifname, ifname))
			{
				printf("ifname have Created\n");
				return E_FOUND;
			}
		}


		libpcap_handler_t *handle = (libpcap_handler_t *)
								malloc(sizeof(libpcap_handler_t));
		if(handle == NULL)
		{	
			return E_FAIL;		
		}
        
        memset(packet_filter, 0, 128);
        snprintf(packet_filter, 128, "%s", NAGA_CONTROL_FILTER);

        char errbuf[PCAP_ERRBUF_SIZE];

        if(ifname== NULL)
            return E_SUCCESS;

        
        /* Open the adapter */
        if ((fp = pcap_open_live(ifname,  // name of the device
                   65536,                 // portion of the packet to capture. It doesn't matter in this case
                     1|                   // promiscuous mode (nonzero means promiscuous)
                     8 |
                     16,
                     0,                     // read timeout
                     errbuf                 // error buffer
            )) == NULL) 
        {
            printf( "Pcap unable open the adapter %s reason is:\n", ifname);
            printf( "%s\n", errbuf );
            BRET(E_FAIL);
        }
        param.fp = fp;
        handle->fp = fp;
		handle->ifname = strdup(ifname);
		
        if (pcap_compile(fp, &fcode, packet_filter, 0, 0) <0 ) 
        {
            printf("Unable to compile the packet filter. Check the syntax.\n");
            BRET(E_FAIL);
        }

        //set the filter
        if (pcap_setfilter(fp, &fcode)<0) 
        {
            printf("Error setting the filter.\n");
            BRET(E_FAIL);
        }

        int rv = pthread_create(&recv_thread, NULL,  pcap_rx_loop, (void *)&param);
		if(rv)
		{
			printf("Failed Create Thread for interface-%s\n", ifname);		
		 	BRET(E_FAIL);
		}
		else
		{
			pthread_detach(recv_thread);
			printf("Success Create Thread-%ld for interface-%s\n",recv_thread, ifname);
		}
		handle->recv_thread = recv_thread;
		list_add_tail(&handle->node, &handle_head);
        return E_SUCCESS;
        
}


berr libpcap_rx_loop_unset(char * ifname __attribute__((unused)))
{
#if 1
	struct list_head *pos = NULL, *next = NULL;
	libpcap_handler_t *handle = NULL;
	//list_for_each_entry_safe(pos, next, (&handle_head), node)
	list_for_each_safe(pos, next,&handle_head)

	{
		handle = (libpcap_handler_t *)list_entry(pos, libpcap_handler_t, node);
		if(!strcmp(handle->ifname, ifname))
		{
			free(handle->ifname);
			pcap_close(handle->fp); 
			list_del(&handle->node);
			pthread_cancle(handle->recv_thread);
			free(handle);
		}
	}
#endif	
    return E_SUCCESS;
}

