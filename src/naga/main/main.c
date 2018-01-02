/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>

#include "main_data.h"
#include "vsr_dp.h"
#include "dmr_dp.h"
#include "acr_dp.h"
#include "cmd.h"
#include "pid.h"
#include "itf.h"
#include "dmr.h"
#include "nag_adp.h"
#include "bts_log.h"
#include "netseg.h"
#include "dnet.h"


char *interface_str = NULL; /*for eth name*/

int
main(int argc, char **argv)
{
    //if(argv[1] != NULL && !strcmp(argv[1], "--NODPDK") )
    {
    	berr rv;
        printf("cmd core %d\n", rte_lcore_id());
        //vsr_dp_init();
		//dmr_dp_init();
		//domain_dp_init();
		//acr_dp_init();
		//netseg_init();
        //adp_dp_init();
        //dnetseg_init();
        //hijack_dp_init();
        //rv = ads_template_init();
        //if (rv )
        //{
        //    printf("%s %d ads_template_init fail, rv(%d)\n", __func__, __LINE__, rv);
        //}


        //injection interface
        //if(argv[2] != NULL)
        //    interface_str = strdup(argv[2]);
        //itf_raw_socket_init(interface_str);
        cmdline (0, NULL);        
    }
	return 0;
}

