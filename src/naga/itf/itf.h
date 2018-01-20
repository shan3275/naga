#ifndef __ITF_H__
#define __ITF_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "boots.h"
#include "naga_types.h"
#include "bts_list.h"

typedef struct
{
	pthread_t recv_thread;
	pcap_t * fp;
	char   *ifname;
	struct list_head node;
}libpcap_handler_t;

extern pcap_t *gpcap_desc;

berr ift_raw_send_packet(uint8_t * buff, int len);
berr itf_raw_socket_init(char *ifname);
void itf_set_hytag_pcap(hytag_t * tag);
berr libpcap_rx_loop_setup(char * ifname);
berr libpcap_rx_loop_unset(char * ifname);
#endif /* !__ITF_H__ */
