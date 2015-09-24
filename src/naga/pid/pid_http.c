#include "pid.h"

#define MAX_METHOD_LEN 64
#define STRING_HTTP_GET  "GET"
#define STRING_HTTP_GET_LEN  3
#define STRING_HTTP_VERSION_MAX  32


static inline berr pid_http_request_method(uint8_t *p ,  uint8_t *method, uint16_t *len)
{
	int i = 0;
	while((0x20 != *p) && ('\0' != *p))    /*GET*/
	{
		if (i + 1 > MAX_METHOD_LEN)
		{
			pid_incr_count(HTTP_METHOD_EXCEED);
			return E_FAIL;
		}
		else
		{
			method[i++] = *(p++);
		}
	}

	method[i] = '\0';
	*len = i;

	return E_SUCCESS;
}



static inline berr pid_http_request_url(uint8_t *p ,  uint8_t *url, uint16_t *len)
{
	int i = 0;
	while((0x20 != *p) && ('\0' != *p))    /*GET*/
	{
		if (i + 1 > URL_MAX_LEN)
		{
			pid_incr_count(HTTP_URL_EXCEED);
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


#if 0
static inline berr pid_http_request_host(uint8_t *p ,  uint8_t *host, uint16_t *len)
{
	int i = 0;
	while(('\r' != *p) && ('\0' != *p))    /*GET*/
	{
		if (i + 1 > MAX_HOST_LEN)
		{
			//pid_incr_count(HTTP_URL_EXCEED);
			return E_FAIL;
		}
		else
		{
			host[i++] = *(p++);
		}
	}

	host[i] = '\0';
	*len = i;

	return E_SUCCESS;
}

#endif

berr pid_http_down(struct pbuf *p ,  hytag_t * hytag )
{
    if( NULL == p || NULL == hytag)
        return E_PARAM;
    return E_SUCCESS;
}


berr pid_http_up(struct pbuf *p ,  hytag_t * hytag )
{
	uint16_t method_len = 0, vsr_len = 0;
	uint16_t len = 0;
	uint8_t *http_p = NULL;
	uint8_t method_http[MAX_METHOD_LEN] = {0};
	char *line = NULL, *begin = NULL, *context_p = NULL;
	
	PBUF_CUR_FORMAT(uint8_t *, http_p, p);

    hytag->app_type = APP_TYPE_HTTP_OTHER; 

	if(E_SUCCESS != pid_http_request_method(http_p, method_http, &method_len))
	{
        printf("ss failed\n");
        /*Tcp syn without method*/
		return E_SUCCESS;
	}

	if ((method_len != STRING_HTTP_GET_LEN ) ||
		(memcmp(STRING_HTTP_GET, method_http, STRING_HTTP_GET_LEN)))
	{
		pid_incr_count(APP_HTTP_OTHER);
		return E_SUCCESS;
	}
	else
	{
		pid_incr_count(APP_HTTP_GET);
	}

	len = method_len + 1;
	UPDATE_PBUF_OFFSET(p, len);
	PBUF_CUR_FORMAT(uint8_t *, http_p, p);

	if (pid_http_request_url(http_p, (uint8_t *)hytag->url, &hytag->url_len))
	{
		//hytag->app_type = URL_IN_NULL;
		;
	}
	else
	{
		hytag->app_type = APP_TYPE_HTTP_GET_OR_POST;
	}
    //printf("the url is:  %s\n", hytag->url);

	len = hytag->url_len + 1;
	UPDATE_PBUF_OFFSET(p, len);
	PBUF_CUR_FORMAT(uint8_t *, http_p, p);

	if(pid_http_request_version_skip(http_p, &vsr_len))
	{
		return E_SUCCESS;
	}

	len = vsr_len + 2;
	UPDATE_PBUF_OFFSET(p, len);
	PBUF_CUR_FORMAT(char *, context_p, p);

	while(NULL != (line = strsep(&context_p, "\n")))
	{
		if (NULL != (begin =strsep(&line, ":")))
		{			
			if (!strcmp("host", begin))	
			{
				memcpy(hytag->host, &line[1], (strlen(line)-2));
				hytag->host_len = strlen(line)-2;
			}
		}
	}
	
	
	printf("the host is:  %s\n", hytag->host);
    return E_SUCCESS;
   
}
#if 0
berr pid_http_get(struct pbuf *p,  hytag_t * hytag)
{

       
}

berr pid_http_post(struct pbuf *p,  hytag_t * hytag)
{

       
}
#endif
