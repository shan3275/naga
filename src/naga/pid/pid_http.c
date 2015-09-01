#include "pid.h"

#define MAX_METHOD_LEN 64
#define STRING_HTTP_GET  "GET"


berr pid_http(struct pbuf *p ,  hytag_t * hytag )
{
	int i = 0, j = 0;
	uint16_t method_len = 0;
	char *http_p = NULL;
	char method_http[MAX_METHOD_LEN] = {0};
	
	PBUF_CUR_FORMAT(char *, http_p, p);

	while((0x20 != *http_p) && (0 != *http_p))
	{
		if (i + 1 < MAX_METHOD_LEN)
		{
			method_http[i++] = *(http_p++);
		}
		else
		{
			//pid_incr_count(NOT_HTTP);
			return E_FAIL;
		}
	}

	method_len = i + 1;
	if ((method_len != strlen(STRING_HTTP_GET)) || 
		(!strncmp(STRING_HTTP_GET, method_http, method_len)))
	{
		//pid_incr_count(HTTP_OTHER);
		return E_FAIL;
	}
	else
	{
		//pid_incr_count(HTTP_GET);
	}

	http_p++;
	http_p++;

	while((0x20 != *http_p) && (0 != *http_p))
	{
		if (j + 1 < URL_MAX_LEN)
		{
			hytag->url[j++] = *(http_p++);
		}
		else
		{
			//pid_incr_count(HTTP_GET_OTHER);
			return E_FAIL;
		}
	}

	printf("the url is:  %s", hytag->url);
	
	hytag->url_len = j + 1;
	hytag->app_type = URL_IN_GTP;
	
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
