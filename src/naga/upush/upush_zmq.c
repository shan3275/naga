/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-03 22:58
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include "zhelpers.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"
#include "naga_types.h"

char *upush_server = NULL;
void *context      = NULL;
void *sender       = NULL;

berr upush_rule_add(char *u_str)
{
    int rv;
    if (u_str == NULL )
    {
        return E_PARAM;
    }

    if (upush_server != NULL)
    {
        return E_EXIST;
    }

    //Socket to receive messages on
    context = zmq_ctx_new ();
    if (context == NULL)
    {
        return E_OPEN;
    }
    //Socket to send messages to
    sender = zmq_socket (context, ZMQ_PUSH);
    if (sender == NULL)
    {
        return E_OPEN;
    }
    rv = zmq_connect (sender, u_str);
    if (rv != 0)
    {
        return E_FAIL;
    }

    upush_server = u_str;

    return E_SUCCESS;
}

berr upush_rule_del(void)
{
    if (upush_server == NULL)
    {
        return E_NULL;
    }
    
    if (context == NULL || sender == NULL)
    {
        return E_EFFECTIVE;
    }

    if (zmq_close (sender))
    {
        return E_FAIL;
    }

    if (zmq_ctx_destroy (context))
    {
        return E_FAIL;
    }

    free(upush_server);
    upush_server = NULL;
    context = NULL;
    sender = NULL;

    return E_SUCCESS;
}

char * upush_rule_get(void)
{
    return upush_server;
}

berr upush_send(char *string)
{
    int size=0;

    if (sender == NULL)
    {
        return E_PARAM;
    }

    CNT_INC(ACL_URLPUSH_TX_PKTS);
    size = s_send (sender, string);        //  Send results to sink
    if (size >= 0)
    {
        if ( size == strlen(string))
        {
            CNT_INC(ACL_URLPUSH_TX_SUCCESS);
        }
        else
        {
            CNT_INC(ACL_URLPUSH_TX_DISMATCH);
        }
        return E_SUCCESS;
    }
    else
    {
        if(errno == 35)
        {
            CNT_INC(ACL_URLPUSH_RESOUR_UNAVA);
        }
        CNT_INC(ACL_URLPUSH_TX_FAIL);
    }
    return E_SUCCESS;
}

berr upush_rule_test(void)
{
    int size=0;

    char *string= "hello world";
    if (sender == NULL)
    {
        return E_PARAM;
    }

    size = s_send (sender, string);        //  Send results to sink
    if (size >= 0)
    {
        return E_SUCCESS;
    }
    else
    {
        if(errno == 35)
        {
            return E_FOUND;
        }
    }

    return E_SUCCESS;
}
