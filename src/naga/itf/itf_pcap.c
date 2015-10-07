#include <stdio.h>
#include "netinet/in.h"
#include <pcap.h>

#include "boots.h"
#include "bts_debug.h"

#include "itf.h"

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
