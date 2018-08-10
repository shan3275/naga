#include <stdio.h>
#include "netinet/in.h"
#include <pcap.h>
#include <pthread.h>
#include "boots.h"
#include "bts_debug.h"
#include <sys/socket.h>
#include <netpacket/packet.h>
#include "itf.h"
#include "boots_custom.h"
#include "dbg_tagmon.h"
#include "itf_stat.h"
#include "bts_cnt.h"
#include "bts_list.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>
#include <fcntl.h>
#include "itf_worker_thread.h"

pcap_t *gpcap_desc = NULL;
static int send_socket = 0;
char if_name[16]={0};
static struct  sockaddr_ll  sll;
int pcap_if_num = 0;

#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  printf("func=%s line=%d:" fmt , __FUNCTION__, __LINE__, ##args)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

berr itf_raw_socket_add(char *ifname)
{
    if(ifname== NULL)
    {
        return E_SUCCESS;
    }

    if (strlen(if_name) > 0)
    {
        return E_EXIST;
    }

    int sockfd = socket(PF_PACKET, SOCK_RAW, 0);
	
    if(sockfd < 0 )
    {
    	printf("create socket Failed\n");
		BRET(E_FAIL);
    }

    struct ifreq ifr;
    socklen_t addrlen = sizeof(sll);
    strcpy(ifr.ifr_name, ifname);
    ioctl(sockfd, SIOCGIFINDEX, &ifr);
    sll.sll_ifindex = ifr.ifr_ifindex; 
    
    sll.sll_family    = AF_PACKET;
    sll.sll_protocol  = htons(ETH_P_ALL);
    sll.sll_pkttype   = PACKET_HOST;
    sll.sll_halen     = 6;
    
    if(bind(sockfd, (struct sockaddr*)&sll, addrlen) < 0)
    {
    	printf("bind socket Failed\n");
		BRET(E_FAIL);			    	
    }

	send_socket = sockfd;
	int flag = fcntl(sockfd, F_GETFL,0);
	printf("flag is %x, %x\n", flag, O_NONBLOCK);
	fcntl(sockfd, F_SETFL, flag & ~O_NONBLOCK);	
    //shutdown(send_socket, SHUT_RD);

    strcpy(if_name, ifname);
    return E_SUCCESS;
}

berr itf_raw_socket_del(char *ifname)
{
    if(ifname== NULL)
    {
        return E_SUCCESS;
    }

    if (strncmp(if_name,ifname, strlen(ifname)) )
    {
        return E_MATCH;
    }

    if (send_socket)
    {
        //if (shutdown(send_socket, SHUT_RDWR))
        if (close(send_socket))
        {
            perror("shutdown:");
            return E_FAIL;
        }
    }
    else
    {
        return E_INIT;
    }

    memset(if_name,0,sizeof(if_name));
    send_socket = 0;
    return E_SUCCESS;
}

berr itf_raw_socket_get_if_name(char *ifname)
{
    if(ifname== NULL)
    {
        return E_SUCCESS;
    }

    strncpy(ifname, if_name, sizeof(if_name));
    return E_SUCCESS;
}

int itf_raw_socket_get_socket(void)
{
    return send_socket;
}


berr ift_raw_send_packet(uint8_t * buff, int len)
{

    //printf("%s.%d: buff = %p, len =%d\n", __func__, __LINE__, buff, len);
	if (itf_tx_is_enable())
	{
		if(send_socket > 0)
		{
			if(sendto(send_socket, buff, len, 0, (const struct sockaddr *)&sll, sizeof(sll))!= len)
			{
				perror("The Err is:");
				return E_FAIL;		
			}
		    else
		    {
		        return E_SUCCESS;   
		    }
		}
		else
		{
		    printf("Socket is %d\n", send_socket);
			return E_FAIL;
		}
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

	if(send_socket != 0)
	{
        tag->eth_tx = ENABLE;	
	}
    return ;
}

extern event_thread_ctx_t  *threads;
extern int Nthreads;

typedef struct
{
    int    id;
    int    last_thread;
    pcap_t *fp;
}libpcap_param_t;

void libpcap_packet_handler(u_char *param ,
                            const struct pcap_pkthdr *pkthdr,  u_char *packet);
extern berr naga_data_process_module(hytag_t * tag);

void libpcap_packet_handler(u_char *param ,
                            const struct pcap_pkthdr *pkthdr,   u_char *packet)
{
    libpcap_param_t *tparam = (libpcap_param_t *)param;
    int tid;
    pthread_testcancel();

    if(pkthdr->caplen <10) return;

	cnt_inc(ITF_IPKTS);
	cnt_add(ITF_IBYTS, pkthdr->len);

	if (!itf_rx_is_enable())
	{
		return ;
	}

    itf_pcap_thread_inc(tparam->id);

    tid = ( (tparam->last_thread) + 1) % Nthreads ;
    tparam->last_thread = tid;
    event_thread_ctx_t* local_thread = threads + tid;

    if (write(local_thread->notify_send_fd, packet, pkthdr->len) != pkthdr->len) 
    {
        perror("Writing to thread notify pipe");
    }

    /* work thread process 
    hytag_t hytag;
    memset(&hytag, 0x0, sizeof(hytag));

    hytag.pbuf.ptr = (void *)packet;
    hytag.pbuf.len = header->len;
    hytag.pbuf.ptr_offset = 0;

	cnt_inc(ITF_IPKTS);
	cnt_add(ITF_IBYTS, header->len);

    naga_data_process_module(&hytag);
    */
    return;
}

void* pcap_rx_loop(void *_param);
void *pcap_rx_loop(void *_param)
{
    libpcap_param_t rparam;
    //when The create Thread exit , The pram will can't assert
    memcpy(&rparam,  _param, sizeof(rparam)); 
    free(_param);

    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0) {
            perror("pthread_setcancelstate err:");
            return NULL;
    }
    if (pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL) != 0) {
            perror("pthread_setcanceltype err:");
            return NULL;
    }

    pcap_loop(rparam.fp, 0, (pcap_handler)libpcap_packet_handler, (void*)&rparam);
    return NULL;
}






//#define NAGA_CONTROL_FILTER "dst port 80"




BTS_LIST_HEAD(handle_head);


berr libpcap_rx_loop_setup(char * ifname)
{
        pcap_t *fp = NULL;
        char packet_filter[128];
        struct bpf_program fcode;
        libpcap_param_t param ;
		pthread_t recv_thread;

		libpcap_handler_t *pos = NULL, *next = NULL;

        /*防止命令行中没有添加worker线程的命令，故缺省添加*/
        itf_worker_thread_setup(Nthreads);

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
        //snprintf(packet_filter, 128, "%s", NAGA_CONTROL_FILTER);

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
        param.id = pcap_if_num++;
        param.last_thread = 0;
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

        libpcap_param_t *tparam = (libpcap_param_t * )malloc(sizeof(libpcap_param_t));

        memcpy(tparam, &param, sizeof(libpcap_param_t));
        if(NULL == tparam)
        {
            BRET(E_FAIL);
        }
        
        int rv = pthread_create(&recv_thread, NULL,  pcap_rx_loop, (void *)tparam);
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
    struct list_head *pos = NULL, *next = NULL;
    libpcap_handler_t *handle = NULL;
    //list_for_each_entry_safe(pos, next, (&handle_head), node)
    list_for_each_safe(pos, next,&handle_head)
    {
        handle = (libpcap_handler_t *)list_entry(pos, libpcap_handler_t, node);
        if(!strcmp(handle->ifname, ifname))
        {

            pcap_close(handle->fp); 
            list_del(&handle->node);
            if(pthread_cancel(handle->recv_thread))
            {
                printf("cancel Thread-%s Failed\n", handle->ifname);
                return E_FAIL;        
            }
            pthread_join(handle->recv_thread, NULL);
            free(handle->ifname);
            free(handle);
        }
    }
    return E_SUCCESS;
}
