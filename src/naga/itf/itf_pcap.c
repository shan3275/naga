#include <stdio.h>
#include "netinet/in.h"
#include <pcap.h>

#include "boots.h"
#include "bts_debug.h"

#include "itf.h"
#include "boots_custom.h"
#include "dbg_tagmon.h"


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
    rv =  pcap_sendpacket((pcap_t *)fp, buff, len); 

    if(rv < 0)
    {
        printf("Send Packet Failed %s %d\n", __func__, __LINE__);
        BRET(E_FAIL);
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
    char buffer[2048];	
	
    memset(&hytag, 0x0, sizeof(hytag));
    memcpy((void *)buffer, (void *)packet, header->len);	
    
    hytag.pbuf.ptr = (void *)buffer;
    hytag.pbuf.len = header->len;
    hytag.pbuf.ptr_offset = 0;
    hytag.m = NULL;
    //printf("Success packet len = %d\n", hytag.pbuf.len);
    //return;
    
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


pthread_t recv_thread;
pcap_t * g_rxfp = NULL;


#define NAGA_CONTROL_FILTER "dst port 80"

berr libpcap_rx_loop_setup(char * ifname)
{
        pcap_t *fp = NULL;
        char packet_filter[128];
        struct bpf_program fcode;
        libpcap_param_t param ;
        
        
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
        g_rxfp = fp;
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

        recv_thread = pthread_create(&recv_thread, NULL,  pcap_rx_loop, (void *)&param);
        pthread_detach(recv_thread);
        return E_SUCCESS;
        
}


berr libpcap_rx_loop_unset(char * ifname __attribute__((unused)))
{
    pcap_close(g_rxfp); 
    g_rxfp = NULL; 
    return E_SUCCESS;
}

