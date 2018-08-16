#include <string.h>

#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"

#include "naga_types.h"
#include "itf.h"
#include "itf_cmd.h"
#include "itf_stat.h"
#include "itf_worker_thread.h"

DEFUN(itf_rxtx, 
      itf_rxtx_cmd,
      "interface bussiness (add|remove|test) IFNAME", "interface setting\nenable or disable\nrx or tx\n")
{
    if( !strcmp (argv[0], "add"))
    {
        char * ifname = strdup(argv[1]);
        berr rv=  libpcap_rx_loop_setup(ifname);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to add %s rx%s", ifname, VTY_NEWLINE);
        else
            vty_out(vty, "Failed to add %s rx%s", ifname, VTY_NEWLINE);
        free(ifname);
        return 0;
    }
	else if(!strcmp (argv[0], "remove"))
	{
        char * ifname = strdup(argv[1]);
        berr rv=  libpcap_rx_loop_unset(ifname);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to remove %s rx%s", ifname, VTY_NEWLINE);
        else
            vty_out(vty, "Failed to remove %s rx%s", ifname, VTY_NEWLINE);
        free(ifname);
        return 0;		
	}
    else if(!strcmp (argv[0], "test"))
    {
        hytag_t hytag;
        memset(&hytag, 0x0, sizeof(hytag));

        char buffer[] = {
            0x00,0x25 ,0x9e,0xf3 ,0x4d,0x3d ,0x08,0x00 ,0x27,0xbd ,0x13,0x60 ,0x08,0x00 ,0x45,0x00, 
        	0x01,0xca ,0xfa,0xb6 ,0x40,0x00 ,0x40,0x06 ,0x89,0x50 ,0xc0,0xa8 ,0x14,0xed ,0x3d,0x87, 
        	0xa2,0x0a ,0x13,0x62 ,0x00,0x50 ,0xd5,0x06 ,0x96,0xc3 ,0x89,0xef ,0x66,0xfd ,0x50,0x18, 
        	0xfa,0xf0 ,0x21,0xa1 ,0x00,0x00 ,0x47,0x45 ,0x54,0x20 ,0x2f,0x20 ,0x48,0x54 ,0x54,0x50, 
        	0x2f,0x31 ,0x2e,0x31 ,0x0d,0x0a ,0x41,0x63 ,0x63,0x65 ,0x70,0x74 ,0x3a,0x20 ,0x61,0x70, 
        	0x70,0x6c ,0x69,0x63 ,0x61,0x74 ,0x69,0x6f ,0x6e,0x2f ,0x78,0x2d ,0x73,0x68 ,0x6f,0x63, 
        	0x6b,0x77 ,0x61,0x76 ,0x65,0x2d ,0x66,0x6c ,0x61,0x73 ,0x68,0x2c ,0x20,0x69 ,0x6d,0x61, 
        	0x67,0x65 ,0x2f,0x67 ,0x69,0x66 ,0x2c,0x20 ,0x69,0x6d ,0x61,0x67 ,0x65,0x2f ,0x78,0x2d, 
        	0x78,0x62 ,0x69,0x74 ,0x6d,0x61 ,0x70,0x2c ,0x20,0x69 ,0x6d,0x61 ,0x67,0x65 ,0x2f,0x6a, 
        	0x70,0x65 ,0x67,0x2c ,0x20,0x69 ,0x6d,0x61 ,0x67,0x65 ,0x2f,0x70 ,0x6a,0x70 ,0x65,0x67, 
        	0x2c,0x20 ,0x61,0x70 ,0x70,0x6c ,0x69,0x63 ,0x61,0x74 ,0x69,0x6f ,0x6e,0x2f ,0x76,0x6e, 
        	0x64,0x2e ,0x6d,0x73 ,0x2d,0x65 ,0x78,0x63 ,0x65,0x6c ,0x2c,0x20 ,0x61,0x70 ,0x70,0x6c, 
        	0x69,0x63 ,0x61,0x74 ,0x69,0x6f ,0x6e,0x2f ,0x76,0x6e ,0x64,0x2e ,0x6d,0x73 ,0x2d,0x70, 
        	0x6f,0x77 ,0x65,0x72 ,0x70,0x6f ,0x69,0x6e ,0x74,0x2c ,0x20,0x61 ,0x70,0x70 ,0x6c,0x69, 
        	0x63,0x61 ,0x74,0x69 ,0x6f,0x6e ,0x2f,0x6d ,0x73,0x77 ,0x6f,0x72 ,0x64,0x2c ,0x20,0x2a, 
        	0x2f,0x2a ,0x0d,0x0a ,0x41,0x63 ,0x63,0x65 ,0x70,0x74 ,0x2d,0x4c ,0x61,0x6e ,0x67,0x75, 
        	0x61,0x67 ,0x65,0x3a ,0x20,0x7a ,0x68,0x2d ,0x63,0x6e ,0x0d,0x0a ,0x41,0x63 ,0x63,0x65, 
        	0x70,0x74 ,0x2d,0x45 ,0x6e,0x63 ,0x6f,0x64 ,0x69,0x6e ,0x67,0x3a ,0x20,0x67 ,0x7a,0x69, 
        	0x70,0x2c ,0x20,0x64 ,0x65,0x66 ,0x6c,0x61 ,0x74,0x65 ,0x0d,0x0a ,0x55,0x73 ,0x65,0x72, 
        	0x2d,0x41 ,0x67,0x65 ,0x6e,0x74 ,0x3a,0x20 ,0x4d,0x6f ,0x7a,0x69 ,0x6c,0x6c ,0x61,0x2f, 
        	0x34,0x2e ,0x30,0x20 ,0x28,0x63 ,0x6f,0x6d ,0x70,0x61 ,0x74,0x69 ,0x62,0x6c ,0x65,0x3b, 
        	0x20,0x4d ,0x53,0x49 ,0x45,0x20 ,0x36,0x2e ,0x30,0x3b ,0x20,0x57 ,0x69,0x6e ,0x64,0x6f, 
        	0x77,0x73 ,0x20,0x4e ,0x54,0x20 ,0x35,0x2e ,0x31,0x3b ,0x20,0x53 ,0x56,0x31 ,0x29,0x0d, 
        	0x0a,0x43 ,0x6f,0x6f ,0x6b,0x69 ,0x65,0x3a ,0x20,0x42 ,0x41,0x49 ,0x44,0x55 ,0x49,0x44, 
        	0x3d,0x36 ,0x30,0x35 ,0x33,0x38 ,0x46,0x32 ,0x43,0x38 ,0x44,0x30 ,0x41,0x30 ,0x37,0x39,
        	0x38,0x45 ,0x42,0x35 ,0x35,0x45 ,0x38,0x43 ,0x41,0x33 ,0x37,0x45 ,0x33,0x31 ,0x34,0x33, 
        	0x31,0x3a ,0x46,0x47 ,0x3d,0x31 ,0x0d,0x0a ,0x43,0x6f ,0x6e,0x6e ,0x65,0x63 ,0x74,0x69, 
        	0x6f,0x6e ,0x3a,0x20 ,0x4b,0x65 ,0x65,0x70 ,0x2d,0x41 ,0x6c,0x69 ,0x76,0x65 ,0x0d,0x0a, 
        	0x48,0x6f ,0x73,0x74 ,0x3a,0x20 ,0x77,0x77 ,0x77,0x2e ,0x68,0x61 ,0x6f,0x31 ,0x32,0x33, 
        	0x2e,0x63 ,0x6f,0x6d ,0x0d,0x0a ,0x0d,0x0a
        };


        hytag.pbuf.ptr = (void *)buffer;
        hytag.pbuf.len =  sizeof(buffer);
        hytag.pbuf.ptr_offset = 0;
        //hytag.m = NULL;



        naga_data_process_module(&hytag);
        vty_out(vty, "Success to test %s", VTY_NEWLINE);
        return;
              
    }
    return 0;
}

DEFUN(itf_injection, 
      itf_injection_cmd,
      "interface injection (add|remove) IFNAME", 
      "interface setting\n"
      "injection cmd\n"
      "add or remove\n"
      "interface name, such as:eth0\n")
{
    if( !strcmp (argv[0], "add"))
    {
        char * ifname = strdup(argv[1]);
        berr rv = itf_raw_socket_add(ifname);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to add %s injection%s", ifname, VTY_NEWLINE);
        else
        if( rv = E_FAIL)
        {
            vty_out(vty, "Failed to add %s %s", ifname, VTY_NEWLINE);
        }
        else
        if( rv = E_EXIST)
        {
            vty_out(vty, "Failed to add %s ,have existed%s", ifname, VTY_NEWLINE);
        }
        free(ifname);
    }
	else if(!strcmp (argv[0], "remove"))
	{
        char * ifname = strdup(argv[1]);
        berr rv = itf_raw_socket_del(ifname);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to remove %s rx%s", ifname, VTY_NEWLINE);
        else
        if( rv = E_FAIL)
        {
            vty_out(vty, "Failed to remove %s%s", ifname, VTY_NEWLINE);
        }
        else
        if( rv = E_MATCH)
        {
            vty_out(vty, "Failed to remove %s interface unmatch%s", ifname, VTY_NEWLINE);
        }
        else
        if( rv = E_INIT)
        {
            vty_out(vty, "Failed to remove %s interface uninit%s", ifname, VTY_NEWLINE);
        }
        free(ifname);
	}
    return 0;
}

DEFUN(itf_work_thread, 
      itf_work_thread_cmd,
      "interface work-thread (add|remove) <2-20>", 
      "interface setting\n"
      "work thread cmd\n"
      "add or remove\n"
      "work thread nunber, don't exceed cores number\n")
{
    berr rv;
    if( !strcmp (argv[0], "add"))
    {
        int nthreads = strtoul(argv[1], NULL, 0 );
        rv = itf_worker_thread_setup(nthreads);
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to add %d work-thread%s", nthreads, VTY_NEWLINE);
        else
        if( rv = E_FAIL)
        {
            vty_out(vty, "Failed to add %d work-thread%s", nthreads, VTY_NEWLINE);
        }
        else
        if( rv = E_INIT)
        {
            vty_out(vty, "Failed to add %d work-thread%s", nthreads, VTY_NEWLINE);
        }
    }
	else if(!strcmp (argv[0], "remove"))
	{
        rv = itf_worker_thread_close();
        if(rv == E_SUCCESS)
            vty_out(vty, "Success to remove work-thread%s", VTY_NEWLINE);
        else
        if( rv = E_FAIL)
        {
            vty_out(vty, "Failed to remove work-thread%s", VTY_NEWLINE);
        }
	}
    return 0;
}
static int itf_cmd_show_status(struct vty *vty)
{
    int rv;
    itf_stat stat;
    stat.direction = ITF_TX;
    rv = itf_stat_get( &stat);
    if (rv)
    {
        vty_out(vty, "get tx status fail rv(%d)%s", rv, VTY_NEWLINE);
    }
    else
    {
        vty_out(vty, "TX :%s%s", stat.enable == ITF_ENABLE ?"Enable":"Disable", VTY_NEWLINE);
    }

    stat.direction = ITF_RX;
    rv = itf_stat_get( &stat);
    if (rv)
    {
        vty_out(vty, "get rx status fail rv(%d)%s", rv, VTY_NEWLINE);
    }
    else
    {
        vty_out(vty, "RX :%s%s", stat.enable == ITF_ENABLE ?"Enable":"Disable", VTY_NEWLINE);
    }


    return CMD_SUCCESS;
}

DEFUN(itf_show_stat, 
      itf_show_stat_cmd,
      "show itf status",
      SHOW_STR
      INTERFACE_STR
      "Status information\n" 
      )
{
    return itf_cmd_show_status(vty);
}


static int itf_cmd_set(struct vty *vty, const char *dir_str, const char *en_str)
{
    int ret = 0;
    itf_stat stat;
    if ('t' == dir_str[0])
    {
        stat.direction = ITF_TX;
    }
    else
    if ('r' == dir_str[0])
    {
        stat.direction = ITF_RX;
    }
    else
    {
        vty_out(vty, "parameter err %s %s", dir_str, VTY_NEWLINE);
        return CMD_WARNING;
    }

    if ('e' == en_str[0])
    {
        stat.enable = ITF_ENABLE;
    }
    else
    if ('d' == en_str[0])
    {
        stat.enable = ITF_DISABLE;
    }
    else
    {
        vty_out(vty, "parameter err %s %s", en_str, VTY_NEWLINE);
        return CMD_WARNING;
    }

    ret = itf_stat_set(&stat);
    if (ret)
    {
        vty_out(vty, "itf stat set fail ret(%d)%s", ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}
DEFUN(itf_set,
      itf_set_cmd,
      "itf (rx|tx) (enable|disable)",
      INTERFACE_STR
      "Rx or Tx direction\n"
      "En or Disable operation\n" 
      )
{
    return itf_cmd_set(vty, argv[0], argv[1]);
}

static int interface_cmd_set(struct vty *vty, const char *if_str, const char *en_str)
{
    int ret = 0;
    port_stat stat;

    stat.port_id = atoi(if_str);
    if ('e' == en_str[0])
    {
        stat.enable = ITF_ENABLE;
    }
    else
    if ('d' == en_str[0])
    {
        stat.enable = ITF_DISABLE;
    }
    else
    {
        vty_out(vty, "parameter err %s %s", en_str, VTY_NEWLINE);
        return CMD_WARNING;
    }

    ret = interface_stat_set(&stat);
    if (ret)
    {
        vty_out(vty, "interface stat set fail ret(%d)%s", ret, VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}

DEFUN(interface_set,
      interface_set_cmd,
      "interface <0-1> (enable|disable)",
      INTERFACE_STR
      "Interface Number\n"
      "En or Disable operation\n" 
      )
{
    return interface_cmd_set(vty, argv[0], argv[1]);
}

DEFUN(interface_show_stat, 
      interface_show_stat_cmd,
      "show interface status",
      SHOW_STR
      INTERFACE_STR
      "Status information\n" 
      )
{
    int rv;
    char ifname[16]={0};
    rv = itf_raw_socket_get_if_name(ifname);
    if (rv == E_SUCCESS)
    {
        vty_out(vty, "interface injection %s%s", ifname, VTY_NEWLINE);
    }
    #if USE_MULTI_RAW_SOCKET
    int i;
    int socket[MAX_WORKER_THREAD_NUM]={0};
    itf_raw_socket_get_socket(socket);
    for (i = 0; i < MAX_WORKER_THREAD_NUM; ++i)
    {
        vty_out(vty, "interface injection socket[%d] %d%s", i, socket[i], VTY_NEWLINE);
    }
    #else
    int socket = 0;
    socket = itf_raw_socket_get_socket();
    vty_out(vty, "interface injection socket %d%s", socket, VTY_NEWLINE);
    #endif
    return CMD_SUCCESS;
    //return interface_cmd_show_status(vty);
}

DEFUN(interface_show_work_thread, 
      interface_show_work_thread_cmd,
      "show interface work-thread",
      SHOW_STR
      INTERFACE_STR
      "Work-thread information\n" 
      )
{
    int rv;
    char buff[512]={0};
    rv = itf_worker_thread_get(buff);
    if (rv == E_SUCCESS)
    {
        vty_out(vty, "idx    thread_id      rx_id      tx_id%s",  VTY_NEWLINE);
        vty_out(vty, "%s", buff);
    }
    else
    {
        vty_out(vty, "interface work-thread is NULL%s",  VTY_NEWLINE);
    }
    memset(buff, 0,512);
    itf_thread_stat_get(buff);
    vty_out(vty, "Thread Statistics:%s", VTY_NEWLINE);
    vty_out(vty, "%s", buff);
    return CMD_SUCCESS;
}

void itf_cmd_config_write(struct vty *vty)
{
    int rv;
    itf_stat stat;
    stat.direction = ITF_TX;
    rv = itf_stat_get( &stat);
    if ( E_SUCCESS == rv)
    {
        vty_out(vty, "itf tx %s%s", stat.enable == ITF_ENABLE ?"enable":"disable", VTY_NEWLINE);
    }

    stat.direction = ITF_RX;
    rv = itf_stat_get( &stat);
    if ( E_SUCCESS == rv)
    {
        vty_out(vty, "itf rx %s%s", stat.enable == ITF_ENABLE ?"enable":"disable", VTY_NEWLINE);
    }

    {
        int i;
        port_stat stat;
        for ( i = 0; i < INTERFACE_NUM_MAX; i++ )
        {
            stat.port_id = i;
            rv = interface_stat_get( &stat);
            if ( E_SUCCESS == rv)
            {
                vty_out(vty, "interface %d %s%s", i, stat.enable == ITF_ENABLE ?"enable":"disable", VTY_NEWLINE);
            }
        }
    }

    int worker_thread_num = itf_worker_thread_num_get();
    vty_out(vty, "interface work-thread add %d%s", worker_thread_num, VTY_NEWLINE);

extern struct list_head	handle_head;
	struct list_head *pos = NULL, *next = NULL;
	libpcap_handler_t *handle = NULL;
	list_for_each_safe(pos, next,&handle_head)
	{
		handle = (libpcap_handler_t *)list_entry(pos, libpcap_handler_t, node);
		vty_out(vty, "interface bussiness add %s%s", handle->ifname, VTY_NEWLINE);			
	}

    char ifname[16]={0};
    rv = itf_raw_socket_get_if_name(ifname);
    if (rv == E_SUCCESS)
    {
        vty_out(vty, "interface injection add %s%s", ifname, VTY_NEWLINE);
    }

}






/*
 * vsr module cmdline register and init 
 *
 * */
void cmdline_itf_init(void)
{
    install_element(CMD_NODE, &itf_rxtx_cmd);
    install_element(CMD_NODE, &itf_injection_cmd);
    install_element(CMD_NODE, &itf_work_thread_cmd);
    install_element(CMD_NODE, &itf_show_stat_cmd);
    install_element(CMD_NODE, &itf_set_cmd);
    install_element(CMD_NODE, &interface_set_cmd);
    install_element(CMD_NODE, &interface_show_stat_cmd);
    install_element(CMD_NODE, &interface_show_work_thread_cmd);

    return ;
}


