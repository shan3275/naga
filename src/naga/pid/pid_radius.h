#ifndef __PID_RADIUS_H_
#define __PID_RADIUS_H_

typedef enum
{
	RADIUS_ACCESS_REQUEST =1,
	RADIUS_ACCESS_ACCEPT  =2,
	RADIUS_ACCOUNTING_REQUEST  = 4,
	RADIUS_ACCOUNTING_RESPONSE = 5, 
    RADIUS_MAX,
}radius_type_t;

struct radius_hdr
{
	uint8_t  code;
	uint8_t  id;
	uint16_t len;
	uint8_t  auth[16];
};

#define RADIUS_CODE(radiushdr) (radiushdr->code)

berr pid_radius(struct pbuf *p, hytag_t *hytag);

#define RADIUS_HEAD_LEN  20


#endif
