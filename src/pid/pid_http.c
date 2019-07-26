#include "pid.h"
//#include "http_parser.h"
#include "naga_util.h"
#include "bts_debug.h"

#define MAX_METHOD_LEN 64
#define STRING_HTTP_GET  "GET"
#define STRING_HTTP_GET_LEN  3
#define STRING_HTTP_VERSION_MAX  32

#define STRING_HTTP_HOST "Host"
#define STRING_HTTP_HOST_LEN 4

#define STRING_HTTP_REFERENCE "Referer"
#define STRING_HTTP_REFERENCE_LEN  7
#define STRING_HTTP_AGENT     "User-Agent"
#define STRING_HTTP_AGENT_LEN 10

#define STRING_HTTP_ICLOUD_DSID 	 "iCloud-DSID"
#define STRING_HTTP_ICLOUD_DSID_LEN  11

#define STRING_HTTP_WECHAT_KEY "x-wechat-key"
#define STRING_HTTP_WECHAT_KEY_LEN 12
#define STRING_HTTP_WECHAT_UIN "x-wechat-uin"
#define STRING_HTTP_WECHAT_UIN_LEN 12

//#define DEBUG
#ifdef  DEBUG   
#define debug(fmt,args...)  printf ("func(%s), line(%d)"fmt"\n" ,__FUNCTION__, __LINE__, ##args)
#else   
#define debug(fmt,args...)   
#endif  /* DEBUG */ 

static inline int pid_http_request_method(uint8_t *p ,  uint8_t *method, uint16_t *len)
{
	int i = 0;
	while((0x20 != *p) && ('\0' != *p))    /*GET*/
	{
		if (i + 1 > MAX_METHOD_LEN)
		{
			pid_incr_count(HTTP_METHOD_EXCEED);
			return FALSE;
		}
		else
		{
			method[i++] = *(p++);
		}
	}

	method[i] = '\0';
	*len = i;

	return TRUE;
}

static inline berr pid_http_request_url(uint8_t *p ,  uint8_t *url, uint16_t *len)
{
	int i = 0;
	while((0x20 != *p) && ('\0' != *p))    /*GET*/
	{
		if (i + 1 > URL_MAX_LEN)
		{
			//pid_incr_count(HTTP_URL_EXCEED);
			//BRET(E_FAIL);
			return E_FAIL;
		}
		else
		{
			url[i++] = *(p++);
		}
	}

	url[i] = '\0';
	*len = i;

	return E_SUCCESS;
}

static inline berr pid_http_request_version_skip(uint8_t *p, uint16_t *len)
{
	int i = 0;
	while(('\r' != *p) && ('\0' != *p)) 
	{
		if (i + 1 > STRING_HTTP_VERSION_MAX)
		{
			//pid_incr_count(HTTP_URL_EXCEED);
			//BRET(E_FAIL);
			return E_FAIL;
		}
		else
		{
			p++;
		}

		i ++;
	}

	*len = i + 1;

	return E_SUCCESS;
}

berr pid_http_down(struct pbuf *p ,  hytag_t * hytag )
{
    if( NULL == p || NULL == hytag)
        BRET(E_PARAM);
    return E_SUCCESS;
}

berr pid_http_up(struct pbuf *p ,  hytag_t * hytag )
{
    int len = 0;
    uint8_t *http_p = NULL;
    char l5payload[PACKET_MTU];
    char *l5_ptr = NULL; 
    char * line = NULL;
    uint16_t l5_len = 0;
    char *method = NULL, *uri = NULL;    
    char *begin = NULL;
 
    l5_ptr = l5payload;
    l5_len = p->len - p->ptr_offset;
    
    PBUF_CUR_FORMAT(uint8_t *, http_p, p);
    if( l5_len <= 0  || l5_len >= PACKET_MTU)
    {
        pid_incr_count(APP_HTTP_OTHER);
	    return E_SUCCESS;
    }	 	
    memcpy(l5_ptr, http_p, l5_len);
    l5_ptr[l5_len] = '\0'; 

    line = strsep(&l5_ptr, "\n");
    if(line != NULL)
    {
        method = strsep(&line, " ");
        if (NULL == method  || strncmp(STRING_HTTP_GET, method, STRING_HTTP_GET_LEN))
        {
        	pid_incr_count(APP_HTTP_OTHER);
        	return E_SUCCESS;
        }
        pid_incr_count(APP_HTTP_GET);
        hytag->app_type = APP_TYPE_HTTP_GET_OR_POST;

        uri = strsep(&line, " ");
        if(uri != NULL)
        {
            strncpy(hytag->uri, uri, URL_MAX_LEN);
            hytag->uri_len = strlen(uri);
        }
        else
        {
            return E_SUCCESS;
        }
        
    }
    else
    {
        return E_SUCCESS;
    }

	
	while(NULL != (line = strsep(&l5_ptr, "\n")))
	{
		if (NULL != (begin = strsep(&line, ":")))
		{	
			
            if( NULL == line)
                continue;

			len = strlen(line);
			if(len <= 2)
			{
				continue;
			}
            else
            {
			    len -= 2;//valid len
            }

			if (hytag->host_len ==0 
                && !strncmp(STRING_HTTP_HOST, begin, STRING_HTTP_HOST_LEN)) 
			{
               
                if( len > MAX_HOST_LEN )
				{	
					len = MAX_HOST_LEN;
				}
           
				memcpy(hytag->host, &line[1], len);
				hytag->host_len = len;
               	continue;
			}

			if (hytag->user_agent_len== 0 
                    && !strncmp(STRING_HTTP_AGENT, begin, STRING_HTTP_AGENT_LEN))   
			{
           
                if( len > MAX_USER_AGENT_LEN )
				{	
					len = MAX_USER_AGENT_LEN;
				}
				memcpy(hytag->user_agent, &line[1], len);
				hytag->user_agent_len = len;
				continue;
			}			

            if (hytag->referer_len== 0 
                    && !strncmp(STRING_HTTP_REFERENCE, begin, STRING_HTTP_REFERENCE_LEN))
            {
                if( len > URL_MAX_LEN )
                {	
                    len = URL_MAX_LEN;
                }
                memcpy(hytag->referer, &line[1], len) ; 
                hytag->referer_len = len;
                continue;
            }

            if (hytag->icloud_dsid_len == 0 && !strncmp(STRING_HTTP_ICLOUD_DSID, begin, STRING_HTTP_ICLOUD_DSID_LEN))
            {
                if( len > MAX_ICLOUD_DSID_LEN )
                {	
                    len = MAX_ICLOUD_DSID_LEN;
                }
                memcpy(hytag->icloud_dsid, &line[1], len) ; 
                hytag->icloud_dsid_len = len; 
                //printf("iCloud-DSID(%d): %s\n", hytag->icloud_dsid_len, hytag->icloud_dsid);
                continue;
            }
#if 0
            if (hytag->wechat_key_len == 0 && !strncmp(STRING_HTTP_WECHAT_KEY, begin, STRING_HTTP_WECHAT_KEY_LEN))
            {
                if( len > MAX_WECHAT_KEY_LEN )
                {	
                    len = MAX_WECHAT_KEY_LEN;
                }
                memcpy(hytag->wechat_key, &line[1], len) ; 
                hytag->wechat_key_len = len; 
                //printf("x-wechat-key(%d): %s\n", hytag->wechat_key_len, hytag->wechat_key);
                continue;
            }

            if (hytag->wechat_uin_len == 0 && !strncmp(STRING_HTTP_WECHAT_UIN, begin, STRING_HTTP_WECHAT_UIN_LEN))
            {
                if( len > MAX_WECHAT_UIN_LEN )
                {	
                    len = MAX_WECHAT_UIN_LEN;
                }
                memcpy(hytag->wechat_uin, &line[1], len) ; 
                hytag->wechat_uin_len = len; 
                //printf("x-wechat-uin(%d): %s\n", hytag->wechat_uin_len, hytag->wechat_uin);
                continue;
            }
#endif
		}
	}

    if(hytag->uri[0] == '/' && hytag->host_len > 0 && hytag->uri_len > 0)
    {
        hytag->url_len= snprintf(hytag->url, URL_MAX_LEN, "%s%s",
                hytag->host, hytag->uri);
    }

    return E_SUCCESS;
}
