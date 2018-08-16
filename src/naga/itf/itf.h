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


typedef struct
{
    int len;
    int rsv;
    u_char packet[2040];
}pcap_pktbuf_t;

extern pcap_t *gpcap_desc;
#define MAX_PCAP_THREAD_NUM 4
#define MAX_WORKER_THREAD_NUM 20
typedef struct
{
    uint64_t pcap_thread_stat[MAX_PCAP_THREAD_NUM];
    uint64_t worker_thread_stat[MAX_WORKER_THREAD_NUM];
    uint64_t worker_thread_fail_stat[MAX_WORKER_THREAD_NUM];
}itf_thread_stat_t;

#if USE_MULTI_RAW_SOCKET
berr ift_raw_send_packet(int idx,uint8_t * buff, int len);
void itf_raw_socket_get_socket(int *socket);
#else
berr ift_raw_send_packet(uint8_t * buff, int len);
int itf_raw_socket_get_socket(void);
#endif

berr itf_raw_socket_add(char *ifname);
berr itf_raw_socket_del(char *ifname);
berr itf_raw_socket_get_if_name(char *ifname);
void itf_set_hytag_pcap(hytag_t * tag);
berr libpcap_rx_loop_setup(char * ifname);
berr libpcap_rx_loop_unset(char * ifname);
void itf_stat_init(void);
void itf_pcap_thread_inc(int id);
void itf_work_thread_inc(int id);
void itf_work_thread_fail_inc(int id);
void itf_thread_stat_get(char *buff);
#endif /* !__ITF_H__ */
