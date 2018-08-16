/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-03 22:59
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "boots.h"
#include "bts_debug.h"
#include "bts_cnt.h"
#include "naga_types.h"
#include "cbase64.h"

//#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

unsigned char key_for_rc4_use[64] = "wStringb";
int upush_wb_businuss_status = 0;

struct rc4_state {
    u_char  perm[256];
    u_char  index1;
    u_char  index2;
};

static __inline void
swap_bytes(u_char *a, u_char *b)
{
    u_char temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

/*
 *  *  * Initialize an RC4 state buffer using the supplied key,
 *   *   * which can have arbitrary length.
 *    *    */
void
rc4_init(struct rc4_state *const state, const u_char *key, int keylen)
{
    u_char j;
    int i;

    /* Initialize state with identity permutation */
    for (i = 0; i < 256; i++)
        state->perm[i] = (u_char)i;
    state->index1 = 0;
    state->index2 = 0;

    /* Randomize the permutation using key data */
    for (j = i = 0; i < 256; i++) {
        j += state->perm[i] + key[i % keylen];
        swap_bytes(&state->perm[i], &state->perm[j]);
    }
}

/*
 *  *  * Encrypt some data using the supplied RC4 state buffer.
 *   *   * The input and output buffers may be the same buffer.
 *    *    * Since RC4 is a stream cypher, this function is used
 *     *     * for both encryption and decryption.
 *      *      */
void
rc4_crypt(struct rc4_state *const state,
    const u_char *inbuf, u_char *outbuf, int buflen)
{
    int i;
    u_char j;

    for (i = 0; i < buflen; i++) {

        /* Update modification indicies */
        state->index1++;
        state->index2 += state->perm[state->index1];

        /* Modify permutation */
        swap_bytes(&state->perm[state->index1],
            &state->perm[state->index2]);

        /* Encrypt/decrypt next byte */
        j = state->perm[state->index1] + state->perm[state->index2];
        outbuf[i] = inbuf[i] ^ state->perm[j];
    }
}


void rc4_process(u_char *Messige, u_char *encryp);
{
    int datalen = strlen(Messige);
    int keylen = strlen(key_for_rc4_use);
    
    struct rc4_state state;
    memset(&state, 0, sizeof(state));

    rc4_init(&state, key_for_rc4_use, keylen);// this code is very important

    rc4_crypt(&state, Messige, encryp, datalen);
}

berr upush_wb_enable_set(int status)
{
    upush_wb_businuss_status = status;
    return E_SUCCESS;
}

berr upush_content_generator(hytag_t *hytag, char *ptr)
{
    int outlen =0;
    int rv = 0;
    char buffer[2048];

    memset(buffer,0,sizeof(buffer));
    if (APP_URLPUSH_IDFA == hytag->acl.push_type)
    {
        sprintf( buffer, "{\"flag\":\"10\",\"srcIP\":\"%d.%d.%d.%d\",\"idfa\":\"%s\"}",
                (hytag->outer_srcip4 >>24) &0xff,
                (hytag->outer_srcip4 >>16) &0xff,
                (hytag->outer_srcip4 >>8 ) &0xff,
                (hytag->outer_srcip4     ) &0xff,
                hytag->reg);
        CNT_INC(ACL_URLPUSH_IDFA);
    }
    else
    if (APP_URLPUSH_APPID == hytag->acl.push_type)
    {
        sprintf( buffer, "{\"flag\":\"20\",\"srcIP\":\"%d.%d.%d.%d\",\"appid\":\"%s\"}",
                (hytag->outer_srcip4 >>24) &0xff,
                (hytag->outer_srcip4 >>16) &0xff,
                (hytag->outer_srcip4 >>8 ) &0xff,
                (hytag->outer_srcip4     ) &0xff,
                hytag->reg);
        CNT_INC(ACL_URLPUSH_APPID);
    }
    else
    if (APP_URLPUSH_WB == hytag->acl.push_type)
    {
        if (0 == upush_wb_businuss_status)
        {
            return E_SUCCESS;
        }
        char uri_encry[512] = {0};
        rc4_process(hytag->uri, uri_encry); 
        sprintf( buffer, "{\"flag\":\"30\",\"srcIP\":\"%d.%d.%d.%d\",\"duri\":\"%s\"}",
                (hytag->outer_srcip4 >>24) &0xff,
                (hytag->outer_srcip4 >>16) &0xff,
                (hytag->outer_srcip4 >>8 ) &0xff,
                (hytag->outer_srcip4     ) &0xff,
                uri_encry);
        //CNT_INC(ACL_URLPUSH_WB);
    }
    else
    {
        return E_PARAM;
    }

    debug("buffer(%s),len(%d)", buffer, strlen(buffer));

    rv =  base64_encode(buffer, strlen(buffer), ptr, &outlen);
    if (rv)
    {
        return E_ENCODE;
    }
    debug("encode buff: %s", ptr);

    return E_SUCCESS;
}

