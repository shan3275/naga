#include "naga.h"



berr pid_L4(struct pbuf *p, hytag_t *hytag,  uint8_t protocol, int outer_inner)
{
	
	switch(protocol)
	{
		case IP_PROTO_UDP:
			return pid_udp(p,  hytag, outer_inner);
			break;
		case IP_PROTO_UDPLITE:
			break;	
         default:
                //count and incr
            break;
	}
	return E_SUCCESS;
}

