/* main routine.
 * Copyright (C) 1997, 98 Kunihiro Ishiguro
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.  
 */

#include "zebra.h"

#include "version.h"
#include "getopt.h"
#include "command.h"
#include "thread.h"
#include "filter.h"
#include "memory.h"
#include "prefix.h"
#include "log.h"
#include "privs.h"
#include "sigevent.h"

#include "zserv.h"

#include "vsr_cmd.h"
#include "dmr_cmd.h"
#include "bts_cmd.h"
#include "acr_cmd.h"
#include "itf_cmd.h"
#include "adt_cmd.h"
#include "netseg_cmd.h"
#include "url_cmd.h"
#include "referer_cmd.h"
#include "hijack_cmd.h"

/* Zebra instance */
struct zebra_t zebrad =
{
    .rtm_table_default = 0,
};

/* process id. */
pid_t pid;

/* zebra_rib's workqueue hold time. Private export for use by test code only */
extern int rib_process_hold_time;

/* Pacify zclient.o in libzebra, which expects this variable. */
struct thread_master *master;

/* Command line options. */
struct option longopts[] = 
{
    { "batch",       no_argument,       NULL, 'b'},
    { "daemon",      no_argument,       NULL, 'd'},
    { "config_file", required_argument, NULL, 'f'},
    { "help",        no_argument,       NULL, 'h'},
    { "vty_addr",    required_argument, NULL, 'A'},
    { "vty_port",    required_argument, NULL, 'P'},
    { "version",     no_argument,       NULL, 'v'},
    { "rib_hold",	   required_argument, NULL, 'r'},
    { 0 }
};

zebra_capabilities_t _caps_p [] = 
{
    ZCAP_NET_ADMIN,
    ZCAP_SYS_ADMIN,
    ZCAP_NET_RAW,
};

/* Default configuration file path. */
char config_default[] = SYSCONFDIR DEFAULT_CONFIG_FILE;

/* Process ID saved for use by init system */
const char *pid_file = PATH_ZEBRA_PID;

/* Help information display. */
    static void
usage (char *progname, int status)
{
    if (status != 0)
        fprintf (stderr, "Try `%s --help' for more information.\n", progname);
    else
    {    
        printf ("Usage : %s [OPTION...]\n\n"\
                "Daemon which manages kernel routing table management and "\
                "redistribution between different routing protocols.\n\n"\
                "-b, --batch        Runs in batch mode\n"\
                "-d, --daemon       Runs in daemon mode\n"\
                "-f, --config_file  Set configuration file name\n"\
                "-A, --vty_addr     Set vty's bind address\n"\
                "-P, --vty_port     Set vty's port number\n"\
                "-r, --rib_hold	  Set rib-queue hold time\n"\
                "-v, --version      Print program version\n"\
                "-h, --help         Display this help and exit\n"\
                "\n"\
                "Report bugs to %s\n", progname, ZEBRA_BUG_ADDRESS);
    }

    exit (status);
}

/* SIGHUP handler. */
    static void 
sighup (void)
{
    zlog_info ("SIGHUP received");

    /* Reload of config file. */
    ;
}

/* SIGINT handler. */
    static void
sigint (void)
{
    zlog_notice ("Terminating on signal");

    exit (0);
}

/* SIGUSR1 handler. */
    static void
sigusr1 (void)
{
    zlog_rotate (NULL);
}

struct quagga_signal_t zebra_signals[] =
{
    { 
        .signal = SIGHUP, 
        .handler = &sighup,
    },
    {
        .signal = SIGUSR1,
        .handler = &sigusr1,
    },
    {
        .signal = SIGINT,
        .handler = &sigint,
    },
    {
        .signal = SIGTERM,
        .handler = &sigint,
    },
};

/* add by SamLiu, aim to
 * 1. timer event；
 * 2. 5 seconds update;
 * 3. run cmd: file log cmd
 * 4. update log file name
 * */
extern struct host host;
extern struct zlog *zlog_default;
extern int zlog_set_file (struct zlog *zl, const char *filename, int log_level);
void update_log_file(void)
{
	 if (host.logfile && (zlog_default->maxlvl[ZLOG_DEST_FILE] != ZLOG_DISABLED))
	 {
		 zlog_set_file (NULL, host.logfile, zlog_default->default_lvl);
	 }
}

int update_log_file_timer(struct thread * t)
{
	update_log_file();
	thread_add_timer(zebrad.master, update_log_file_timer, NULL, 5);
}

/* update naga stat, refer bts_cmd.c */
int bts_stat_log_file_timer(struct thread *t)
{
	bts_stat_log();
	thread_add_timer(zebrad.master, bts_stat_log_file_timer, NULL, 60);
}

/* cmdline  startup routine. */
//int cmdline (int argc, char **argv)
int cmdline (int argc, char **argv)
{
    char *p;
    char *vty_addr = "127.0.0.1";
    int vty_port = ZEBRA_VTY_PORT;
    int batch_mode = 0;
    int daemon_mode = 0;
    char *config_file = "zebra.conf";
    char *progname = "naga";
    struct thread thread;

    /* Set umask before anything for security */
    umask (0027);

    zlog_default = openzlog (progname, ZLOG_ZEBRA,
            LOG_CONS|LOG_NDELAY|LOG_PID, LOG_DAEMON);

    /* port and conf file mandatory */
    if (!vty_port || !config_file)
    {
        fprintf (stderr, "Error: --vty_port and --config_file arguments"
                " are both required\n");
        usage (progname, 1);
    }

    /* Make master thread emulator. */
    zebrad.master = thread_master_create ();

    /* Vty related initialize. */
    signal_init (zebrad.master, array_size(zebra_signals), zebra_signals);
    cmd_init (1);
    vty_init (zebrad.master);
    //memory_init ();
    //comment by shan275
    //cmdline_vsr_init();
    cmdline_bts_init();
	//cmdline_dmr_init();
	//cmdline_domain_init();
	//cmdline_acr_init();
    //cmdline_adp_init();
    cmdline_itf_init();
    //cmdline_adt_init();
	//cmdline_netseg_init();
	//cmdline_dnetseg_init();
	cmdline_url_init();
	cmdline_referer_init();
    //cmdline_urlr_init();
    //cmdline_hijack_init();
    /* Zebra related initialize. */
    //access_list_init ();


    /* Configuration file read*/
    vty_read_config (config_file, config_default);


    /* Exit when zebra is working in batch mode. */
    if (batch_mode)
        exit (0);

    /* Daemonize. */
    if (daemon_mode && daemon (0, 0) < 0)
    {
        perror("daemon start failed");
        exit (1);
    }

    /* Needed for BSD routing socket. */
    pid = getpid ();

    /* Make vty server socket. */
    vty_serv_sock (vty_addr, vty_port, "/tmp/test_zebra");

    /* Print banner. */
    zlog_notice ("Zebra %s starting: vty@%d", QUAGGA_VERSION, vty_port);
    printf ("Zebra %s starting: vty@%d", QUAGGA_VERSION, vty_port);
    fflush(0);
    /* add by Samliu*/
    //thread_add_timer(zebrad.master, update_log_file_timer, NULL, 5);
    thread_add_timer(zebrad.master, bts_stat_log_file_timer, NULL, 8);

    while (thread_fetch (zebrad.master, &thread))
        thread_call (&thread);

    /* Not reached... */
    return 0;
}
