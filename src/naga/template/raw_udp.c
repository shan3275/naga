#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/in.h>
#include "raw_head.h"
#include "naga_types.h"

//#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

//caculate ip checksum
unsigned short int ip_csum (unsigned short int *addr, int len)
{
    int nleft = len;
    int sum = 0;
    unsigned short int *w = addr;
    unsigned short int answer = 0; 
    while (nleft > 1) 
    {
        sum += *w++;
        nleft -= sizeof (unsigned short int);
    }

    if (nleft == 1) 
    {
        *(char *) (&answer) = *(char *) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

//caculate udp checksum (include payload)
unsigned short int udp_csum (struct raw_ipheader iphdr, struct raw_udpheader udphdr, unsigned char *payload, int payloadlen)
{
    char buf[65535];
    char *ptr;
    int chksumlen = 0;
    int i;

    ptr = &buf[0];  // ptr points to beginning of buffer buf

    /****************pseudo header  12byte*********************************/

    // Copy source IP address into buf (32 bits)
    memcpy (ptr, &iphdr.iph_sourceip, sizeof (iphdr.iph_sourceip));
    ptr += sizeof (iphdr.iph_sourceip);
    chksumlen += sizeof (iphdr.iph_sourceip);

    // Copy destination IP address into buf (32 bits)
    memcpy (ptr, &iphdr.iph_destip, sizeof (iphdr.iph_destip));
    ptr += sizeof (iphdr.iph_destip);
    chksumlen += sizeof (iphdr.iph_destip);

    // Copy zero field to buf (8 bits)
    *ptr = 0; ptr++;
    chksumlen += 1;

    // Copy transport layer protocol to buf (8 bits)
    memcpy (ptr, &iphdr.iph_protocol, sizeof (iphdr.iph_protocol));
    ptr += sizeof (iphdr.iph_protocol);
    chksumlen += sizeof (iphdr.iph_protocol);

    // Copy UDP length to buf (16 bits)
    memcpy (ptr, &udphdr.udph_len, sizeof (udphdr.udph_len));
    ptr += sizeof (udphdr.udph_len);
    chksumlen += sizeof (udphdr.udph_len);

    /*******************end pseudo header*************************/

    // Copy UDP source port to buf (16 bits)
    memcpy (ptr, &udphdr.udph_srcport, sizeof (udphdr.udph_srcport));
    ptr += sizeof (udphdr.udph_srcport);
    chksumlen += sizeof (udphdr.udph_srcport);

    // Copy UDP destination port to buf (16 bits)
    memcpy (ptr, &udphdr.udph_destport, sizeof (udphdr.udph_destport));
    ptr += sizeof (udphdr.udph_destport);
    chksumlen += sizeof (udphdr.udph_destport);

    // Copy UDP length again to buf (16 bits)
    memcpy (ptr, &udphdr.udph_len, sizeof (udphdr.udph_len));
    ptr += sizeof (udphdr.udph_len);
    chksumlen += sizeof (udphdr.udph_len);

    // Copy UDP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    *ptr = 0; ptr++;
    *ptr = 0; ptr++;
    chksumlen += 2;

    // Copy payload to buf
    memcpy (ptr, payload, payloadlen);
    ptr += payloadlen;
    chksumlen += payloadlen;

    // Pad to the next 16-bit boundary
    for (i=0; i<payloadlen%2; i++, ptr++) {
        *ptr = 0;
        ptr++;
        chksumlen++;
    }

    return ip_csum ((unsigned short int *) buf, chksumlen);
}

extern uint32_t rpush_ip;
extern uint16_t rpush_port;
extern int      ads_mac_enable[2];
extern uint8_t  ads_mac[2][6];

berr raw_udp_content_generator(void *buffer, char * payload, int *len, hytag_t *hytag)
{
    struct raw_ether ether;
    struct raw_ipheader ipheader;
    struct raw_udpheader udpheader;
    struct raw_ether *eth = &ether;
    struct raw_ipheader *ip = &ipheader;
    struct raw_udpheader *udp = &udpheader;

    if(rpush_ip == 0 || rpush_port == 0)
    {
        return E_NULL;
    }


    /********************fill ip header****************************/
    //fill ip header (20 bytes)
    ip->iplv = 1<<6 | 5;//version v4 or v6 and head_len 5 
    //ip首部长度以32bit为单位计算
    ip->iph_tos = 0; // Low delay
    ip->iph_len = htons(sizeof(struct raw_ipheader) + sizeof(struct raw_udpheader) +strlen(payload));
    ip->iph_ident = 0;//标示字段 唯一标示一个数据包
    ip->iph_offset = 0x0040;//16bit include offset and flag
    ip->iph_ttl = 64; // time to live
    ip->iph_protocol = 17; // UDP
    // Source IP address, can use spoofed address here!!!
    ip->iph_sourceip = htonl(hytag->outer_srcip4);
    // The destination IP address
    ip->iph_destip = htonl(rpush_ip);

    // Calculate the checksum for integrity
    //ip->iph_chksum = ip_csum((unsigned short int *)ip, (int)sizeof(struct raw_ipheader));
    ip->iph_chksum = 0;
    //ip->iph_chksum = ip_csum((unsigned short int *)ip, (int)sizeof(struct raw_ipheader) + sizeof(struct raw_udpheader) +strlen(payload));
    ip->iph_chksum = ip_csum((unsigned short int *)ip, (int)sizeof(struct raw_ipheader));
    /******************end fill ip header**********************/
    
    /*****************fill udp header*********************************/
    //udp header
    udp->udph_srcport = htons(hytag->outer_srcport);
    udp->udph_destport = htons(rpush_port);
    udp->udph_len = htons(sizeof(struct raw_udpheader)+strlen(payload));
    udp->udph_chksum = 0;
    udp->udph_chksum =udp_csum(*ip,*udp,payload,strlen(payload));//options
    /*******************end fill udp header**************************/

    /********************fill eth header*********************/
    //  Fill the eth header (14bytes)
    if(ads_mac_enable[0])
    {
        memcpy(eth->dmac, ads_mac[0], 6);
    }
    else
    {
        return E_NULL;
    }

    if(ads_mac_enable[1])
    {
        memcpy(eth->smac, ads_mac[1], 6);
    }
    else
    {
        return E_NULL;
    }
    eth->eth_typ_len[0] = 0x08;
    eth->eth_typ_len[1] = 0x00;

    /***********************end fill eth header*****************/
    memcpy(buffer,eth,sizeof(struct raw_ether));
    memcpy(buffer   + sizeof(struct raw_ether),ip,sizeof(struct raw_ipheader));
    memcpy(buffer   + sizeof(struct raw_ether) +  sizeof(struct raw_ipheader),udp,sizeof(struct raw_udpheader) );
    memcpy(buffer   + sizeof(struct raw_ether) +  sizeof(struct raw_ipheader) + sizeof(struct raw_udpheader),payload,strlen(payload));
    *len = sizeof(struct raw_ether) + sizeof(struct raw_ipheader) + sizeof(struct raw_udpheader) + strlen(payload);
    return E_SUCCESS;
}
void PrintBuffer(void* pBuff, unsigned int nLen)
{
    int i;
    if (NULL == pBuff || 0 == nLen)
    {
        return;
    }
    #define  nBytePerLine  16
    unsigned char* p = (unsigned char*)pBuff;
    char szHex[3*nBytePerLine+1] = {0};

    printf("-----------------begin-------------------\n");
    for ( i=0; i<nLen; ++i)
    {
        int idx = 3 * (i % nBytePerLine);
        if (0 == idx)
        {
            memset(szHex, 0, sizeof(szHex));
        }
#ifdef WIN32
        sprintf_s(&szHex[idx], 4, "%02x ", p[i]);// buff长度要多传入1个字节
#else
        snprintf(&szHex[idx], 4, "%02x ", p[i]); // buff长度要多传入1个字节
#endif


        if (0 == ((i+1) % nBytePerLine))
        {
            printf("%s\n", szHex);
        }
    }

    if (0 != (nLen % nBytePerLine))
    {
        printf("%s\n", szHex);
    }

    printf("------------------end-------------------\n");
}
berr raw_udp_test(void)
{
    berr rv;
    char *payload   = "hello,world";
    char buff[2048] = {0};
    int len = 0;
    hytag_t hytag;
    memset(&hytag,0,sizeof(hytag_t));
    hytag.outer_srcport = 0x1234;
    hytag.outer_srcip4  = 0xc0a80105;

    rv = raw_udp_content_generator(buff, payload, &len, &hytag);
    if(rv != E_SUCCESS)
    {
        debug("raw_udp_content_generator rv =%d", rv);
        return rv;
    }
    debug("rwa_udp_content_generator success");
    debug("len:%d\n", len);
    //PrintBuffer(buff, len);
    #if USE_MULTI_RAW_SOCKET
    rv = ift_raw_send_packet(0, buff, len);
    #else
    rv = ift_raw_send_packet(buff, len);
    #endif
    if(rv != E_SUCCESS)
    {
        debug("ift_raw_send_packet rv =%d", rv);
        return rv;
    }
    debug("rv =%d", rv);
    return E_SUCCESS;
}
