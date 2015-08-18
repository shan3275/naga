#ifndef __PID_IP_H
#define __PID_IP_H

#include <sys/types.h>


/* The IPv4 header. */
struct ip_hdr {
#if BYTE_ORDER == LITTLE_ENDIAN
  u8_t tclass1:4, v:4;
  u8_t flow1:4, tclass2:4;  
#else
  u8_t v:4, tclass1:4;
  u8_t tclass2:8, flow1:4;
#endif
  u16_t flow2;
  u16_t len;                	/* payload length */
  u8_t nexthdr;             	/* next header */
  u8_t hoplim;              	/* hop limit (TTL) */
  struct ip_addr src, dest;     /* source and destination IP addresses */
};




#if BYTE_ORDER == BIG_ENDIAN
/** Set an IP address given by the four byte-parts */
#define IP4_ADDR(ipaddr, a,b,c,d) \
        (ipaddr)->addr = ((u32_t)((a) & 0xff) << 24) | \
                         ((u32_t)((b) & 0xff) << 16) | \
                         ((u32_t)((c) & 0xff) << 8)  | \
                          (u32_t)((d) & 0xff)
#else
/** Set an IP address given by the four byte-parts.
    Little-endian version that prevents the use of htonl. */
#define IP4_ADDR(ipaddr, a,b,c,d) \
        (ipaddr)->addr = ((u32_t)((d) & 0xff) << 24) | \
                         ((u32_t)((c) & 0xff) << 16) | \
                         ((u32_t)((b) & 0xff) << 8)  | \
                          (u32_t)((a) & 0xff)
#endif





berr pid_outerip(struct pbuf *p,  hytag_t * hytag);



#endif

