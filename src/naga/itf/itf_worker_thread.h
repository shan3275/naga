#ifndef __WORKER_THREAD_
#define __WORKER_THREAD_
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <event.h>
#include <pcap.h>
#include <errno.h> 
#include <string.h>
#include "boots.h"

typedef struct {
	int idx ;
	pthread_t thread_id;
	struct event_base *base; //every thread own event base
	struct event notify_event; //notify event	
	int notify_receive_fd;// pipe recv
	int notify_send_fd; //pipe send 
} event_thread_ctx_t;

berr  itf_worker_thread_setup(int nthreads);
berr  itf_worker_thread_close(void);
berr  itf_worker_thread_get(char *buff);
int itf_worker_thread_num_get(void);

#endif
