#include "pid.h"

#define MAX_METHOD_LEN 64
#define STRING_HTTP_GET  "GET"
#define STRING_HTTP_GET_LEN  3

berr pid_http(struct pbuf *p ,  hytag_t * hytag )
{
	int i = 0, j = 0;
	uint16_t method_len = 0;
	unsigned char *http_p = NULL;
	unsigned char method_http[MAX_METHOD_LEN] = {0};
	
	PBUF_CUR_FORMAT(unsigned char *, http_p, p);

	while((0x20 != *http_p) && ('\0' != *http_p))    /*GET*/
	{
		if (i + 1 > MAX_METHOD_LEN)
		{
			pid_incr_count(HTTP_METHOD_EXCEED);
			return E_FAIL;
		}
		else
		{
			method_http[i++] = *(http_p++);
		}
	}

	
    method_http[i] = '\0';
	method_len = i;

	printf("the method is:  %s<%d>\n", method_http, method_len);

	if ((method_len != STRING_HTTP_GET_LEN ) ||
		(memcmp(STRING_HTTP_GET, method_http, STRING_HTTP_GET_LEN)))
	{
		pid_incr_count(APP_HTTP_OTHER);
		return E_FAIL;
	}
	else
	{
		pid_incr_count(APP_HTTP_GET);
	}

	http_p++;
	http_p++;

	while((0x20 != *http_p) && ('\0' != *http_p))
	{
		if (j + 1 > URL_MAX_LEN)
		{
			hytag->app_type = URL_IN_NULL;
			pid_incr_count(HTTP_URL_EXCEED);
			return E_FAIL;
		}
		else
		{
			hytag->url[j++] = *(http_p++);
		}
	}

    hytag->url[j] = '\0';
	printf("the url is:  %s\n", hytag->url);
	
	hytag->url_len = j;
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
