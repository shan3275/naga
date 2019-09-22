#include "pid.h"
#include "pid_radius.h"

//#define DEBUG
#ifdef  DEBUG
#define debug(fmt,args...)  printf("func=%s line=%d:" fmt , __FUNCTION__, __LINE__, ##args)
#else
#define debug(fmt,args...)
#endif  /* DEBUG */

berr pid_radius(struct pbuf *p, hytag_t *hytag)
{
	struct radius_hdr *radiushdr;
    
	if(check_pbuf_len(p, RADIUS_HEAD_LEN))
	{
		pid_incr_count(RADIUS_ERR);//drop packet and incr counter, then return;
		BRET(E_EXCEED);
	}

	PBUF_CUR_FORMAT(struct radius_hdr *, radiushdr, p);	
	radiushdr->len = ntohs(radiushdr->len);
	debug("code:%d, id:%d, len:%d\n", 
		radiushdr->code, radiushdr->id, radiushdr->len);

	if(RADIUS_CODE(radiushdr) != RADIUS_ACCOUNTING_REQUEST )
	{
		pid_incr_count(RADIUS_OTHER);
		BRET(E_FAIL);
	}
    pid_incr_count(RADIUS_ACC);
    UPDATE_PBUF_OFFSET(p, RADIUS_HEAD_LEN);

    uint16_t i = 0;
    uint16_t nLen = radiushdr->len - 20;
	u_char * pAttribute;
	PBUF_CUR_FORMAT(u_char *, pAttribute, p);
	uint8_t nType, nTagLen;
	u_char szBuff[128] = {0}; 
	uint8_t flag = 0;
	while(i < nLen)
	{
		debug("i:%u, nLen:%u\n", i, nLen);
		nType 	= pAttribute[i];
		nTagLen = pAttribute[i+1];
		if( nType == 0 || nTagLen == 0)
		{
			//error, statistics
			break;
		}
		if(NULL == (pAttribute+i+2))
		{	
			return -1;
		}
		if(nTagLen -2 <= 0)
		{
			return -1;
		}
		if (nTagLen >= 66)
		{
			i += nTagLen;
			continue;
		}
		memset(szBuff, 0, 128);
		memcpy(szBuff,  pAttribute+i+2 , nTagLen - 2);
		debug("type:%u, len: %u, szBuff: %s\n", nType, nTagLen, szBuff);

		//if(strlen(szBuff) != 0)
		{
			if(nType == 40)
			{	
				flag = flag + 1;
				hytag->radius_status = szBuff[3];
			}
			if(nType == 1)
			{
				flag = flag + 1;
				hytag->radius_user_name_len = nTagLen;	
				strncpy(hytag->radius_user_name, (char *)szBuff, nTagLen);
			}

			if(nType == 8)
			{
				flag = flag + 1;
				hytag->radius_framed_ip_len = sprintf(hytag->radius_framed_ip, "%u.%u.%u.%u",
					szBuff[0],szBuff[1],szBuff[2],szBuff[3]);
				debug("radius_framed_ip_len:%d, radius_framed_ip:%s\n", hytag->radius_framed_ip_len,
					hytag->radius_framed_ip);
			}

			if (flag == 3)
			{
				break;
			}

		}
		i += nTagLen;
	}
	 if (flag == 3)
	 {
		 pid_incr_count(RADIUS_USER_IP);
	     hytag->app_type = APP_TYPE_RADIUS;	 	
	 }
	 else
	 {
		 pid_incr_count(RADIUS_DROP);	 	
	 }

    return E_SUCCESS;
}


