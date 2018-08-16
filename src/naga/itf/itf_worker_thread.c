#define _GNU_SOURCE
#include <sched.h>
#include "itf_worker_thread.h"
#include <errno.h> 
#include "naga_types.h"
#include "itf_stat.h"
#include "bts_cnt.h"
#include "boots_custom.h"
#include "itf.h"

/*
 * Worker thread: main event loop
 */
static void *worker_func(void *arg) {
    event_thread_ctx_t *me = arg;
    long thread_id = me->idx;
    u_int numCPU = sysconf(_SC_NPROCESSORS_ONLN );
    u_long core_id = (thread_id+1) % numCPU;
    //printf("cpu nums = %d,  current thread id = %ld, bind cpu core id = %ld \n", numCPU, thread_id, core_id);

    if(numCPU > 1)
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        //下面的函数需要在libc2.3.4以后的版本才能使用 
        /*if(pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) < 0 )
          {
          printf("Error while binding thread %ld to core %ld \n", thread_id, core_id );
          }
          else
          {
          printf("Set thread %lu on core %lu/%u\n", thread_id, core_id, numCPU);
          }*/
    }

    //这个方法主要是开启事件的循环
    //每个线程中都会有自己独立的event_base和事件的循环机制
    event_base_loop(me->base, 0);  //每个线程开启自己各自的线程循环读取从主线程中分发过来的数据的
    return NULL;
}


/*
 * Creates a worker thread.
 */
static void create_worker(void *(*func)(void *), void *arg) {
    event_thread_ctx_t *me = arg;
    pthread_t       thread;
    pthread_attr_t  attr;
    int             ret;
    pthread_attr_init(&attr);
    if ((ret = pthread_create(&thread, &attr, func, arg)) != 0) {
        fprintf(stderr, "Can't create thread: %s\n", strerror(ret));
        exit(1);
    }
    me->thread_id = thread;
}

//测试用的
static void transhex( unsigned char* ucParam, int nlen)
{
    //char dest[128] = {0}; 
    int i, j;
    for(i=0, j=0; i<nlen; ++i)
    {
        printf( "%02x", ucParam[i]);
        if(i != 0 && i % 20 == 0 )
            printf("\n");
        //dest +=  2;
    }
    printf("\n");
}

//主线程中有数据往pipe中写入数据以后，就会调用下面的函数
static void thread_libevent_process(int fd, short which, void *arg)
{
    event_thread_ctx_t *me = (event_thread_ctx_t *)arg;
    pcap_pktbuf_t pkt;
    hytag_t hytag;

    memset(&pkt, 0,sizeof(pcap_pktbuf_t));
    //回调函数中回去读取pipe中的信息
    //主线程中如果有新的连接，会向其中一个线程的pipe中写入1
    //这边读取pipe中的数据，如果为1，则说明从pipe中获取的数据是正确的
    int nsize = read(fd, (void *)&pkt, sizeof(pcap_pktbuf_t));
    if(nsize < 0)
    {
        itf_work_thread_fail_inc(me->idx);
        return;
    }

    itf_work_thread_inc(me->idx);

    //printf("thread idx = %d , current id : %ld, recv datalen = %d\n",me->idx,  me->thread_id, nsize);
    //transhex( buf, nsize);

    /* work thread process */
    memset(&hytag, 0x0, sizeof(hytag));

    hytag.pbuf.ptr = (void *)pkt.packet;
    hytag.pbuf.len = pkt.len;
    hytag.pbuf.ptr_offset = 0;
    #if USE_MULTI_RAW_SOCKET 
    hytag.idx = me->idx;
    #endif

    naga_data_process_module(&hytag);
}
//下面是启动一个工作线程
static void setup_thread(event_thread_ctx_t *me) {

    me->base = event_init();  //每个独立的线程都应该有自己独立的event_base
    if (! me->base) {
        fprintf(stderr, "Can't allocate event base\n");
        exit(1);
    }

    /* Listen for notifications from other threads */
    //当pipe中有写入事件的时候，libevent就会回调thread_libevent_process方法
    event_set(&me->notify_event, me->notify_receive_fd, EV_READ | EV_PERSIST, thread_libevent_process, me);
    event_base_set(me->base, &me->notify_event);
    //添加事件操作
    if (event_add(&me->notify_event, 0) == -1) {
        fprintf(stderr, "Can't monitor libevent notify pipe\n");
        exit(1);
    }
}

event_thread_ctx_t  *threads=NULL;
int Nthreads = 2;

event_thread_ctx_t* worker_thread_init(int nthreads) 
{
    int i;
    event_thread_ctx_t * threads = (event_thread_ctx_t*)calloc( nthreads, sizeof( event_thread_ctx_t ) );


    for (i = 0; i < nthreads; i++) {
        int fds[2];
        //这边会创建pipe，主要用于主线程和工作线程之间的通信
        if (pipe(fds)) {
            perror("Can't create notify pipe");
            return NULL;
        }
        threads[i].idx = i;
        //threads是工作线程的基本结构：LIBEVENT_THREAD
        //将pipe接收端和写入端都放到工作线程的结构体中
        threads[i].notify_receive_fd = fds[0]; //接收端
        threads[i].notify_send_fd = fds[1]; //写入端

        //这个方法非常重要，主要是创建每个线程自己的libevent的event_base
        setup_thread(&threads[i]);
        /* Reserve three fds for the libevent base, and two for the pipe */
        //stats.reserved_fds += 5;
    }

    /* Create threads after we've done all the libevent setup. */
    //这里是循环创建线程
    //线程创建的回调函数是worker_libevent
    for (i = 0; i < nthreads; i++) {
        create_worker(worker_func, &threads[i]);
    }

    return threads;
}

berr  itf_worker_thread_setup(int nthreads)
{
    if(threads != NULL)
    {
        return E_INIT;
    }
    event_thread_ctx_t * tmp_thread = NULL;
    tmp_thread = worker_thread_init(nthreads);
    if (tmp_thread == NULL)
    {
        return E_FAIL;
    }
    threads = tmp_thread;
    Nthreads = nthreads;

    return E_SUCCESS;
}

berr itf_worker_thread_close(void)
{
    int i;
    for(i =0 ;i<Nthreads;i++)
    {
        if(pthread_cancel(threads[i].thread_id))
        {
            printf("cancel thread-%ld failed\n", threads[i].thread_id);
            return E_FAIL;
        }
        if(pthread_join(threads[i].thread_id, NULL))
        {
            printf("join thread-%ld failed\n", threads[i].thread_id);
            return E_FAIL;
        }
        event_base_free(threads[i].base);
        threads[i].base = NULL;
    }
    free(threads);
    threads = NULL;
    return E_SUCCESS;
}

berr itf_worker_thread_get(char *buff)
{
    int i;
    int l = 0;
    if(threads == NULL)
    {
        return E_NULL;
    }
    for(i =0 ;i<Nthreads;i++)
    {
        l += snprintf(buff+l, 512-l, "%d       %lld       %d       %d\n\r", threads[i].idx,
                threads[i].thread_id, threads[i].notify_receive_fd, threads[i].notify_send_fd);
    }
    return E_SUCCESS;
}

int itf_worker_thread_num_get(void)
{
    return Nthreads;
}
