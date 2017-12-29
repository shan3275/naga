#include "pid.h"

#define MAX_METHOD_LEN 64
#define STRING_HTTP_GET  "GET"
#define STRING_HTTP_GET_LEN  3
#define STRING_HTTP_VERSION_MAX  32

#define STRING_HTTP_HOST "Host"
#define STRING_HTTP_HOST_LEN 4
#define STRING_HTTP_LOCATION "Location"
#define STRING_HTTP_LOCATION_LEN 8

#define STRING_HTTP_REFERENCE "Referer"
#define STRING_HTTP_REFERENCE_LEN  7
#define STRING_HTTP_AGENT     "User-Agent"
#define STRING_HTTP_AGENT_LEN 10

#define STRING_HTTP_200OK           "HTTP/1.1 200 OK"
#define STRING_HTTP_200OK_LEN       15
#define STRING_HTTP_302_FOUND       "HTTP/1.1 302 Found"
#define STRING_HTTP_302_FOUND_LEN   18

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

    uint8_t *http_p = NULL;
    char l5payload[PACKET_MTU];
    char *l5_ptr = NULL; 
    uint16_t l5_len = 0;
    char * line = NULL;
    char *begin = NULL;

    l5_ptr = l5payload;
    l5_len = hytag->l5_len;
    DEBUG_PRINTF("l5_len: %d\n", l5_len);

    PBUF_CUR_FORMAT(uint8_t *, http_p, p);
    if( l5_len <= 0  || l5_len >= PACKET_MTU)
    {
        pid_incr_count(APP_HTTP_OTHER);
	    return E_SUCCESS;
    }
    memcpy(l5_ptr, http_p, l5_len);
    l5_ptr[l5_len] = '\0'; 
    DEBUG_PRINTF("HTTP content: %s\n", l5_ptr);

    if (!strncmp(STRING_HTTP_302_FOUND, l5_ptr, STRING_HTTP_302_FOUND_LEN))
    {
        DEBUG_PRINTF("302 Hit!!!\n");
        pid_incr_count(APP_HTTP_302);
        pid_add_count(APP_HTTP_302_BYTES, hytag->pbuf.len);
        hytag->app_type = APP_TYPE_HTTP_302;
	    int len = 0;
        /* find out the location*/
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

                if (hytag->location_len ==0 
                        && !strncmp(STRING_HTTP_LOCATION, begin, STRING_HTTP_LOCATION_LEN)) 
                {

                    if( len > LOCATION_MAX_LEN)
                    {	
                        len = LOCATION_MAX_LEN;
                    }
                    memcpy(hytag->location, &line[1], len);
                    hytag->location_len = len;
                }
            }
        }
        return E_SUCCESS;
    }

    DEBUG_PRINTF("200-OK Hit!!!\n");
    pid_incr_count(APP_HTTP_200);
    pid_add_count(APP_HTTP_200_BYTES, hytag->pbuf.len);
    hytag->app_type = APP_TYPE_HTTP_200OK;
#ifdef HTTP_BLOCK
    hytag->http_block.buff_len = l5_len;
    memcpy(hytag->http_block.buff, http_p, l5_len);
    hytag->http_block.buff[l5_len] = '\0'; 
#endif

    return E_SUCCESS;
}

berr pid_http_up(struct pbuf *p ,  hytag_t * hytag )
{
    
    uint8_t *http_p = NULL;
    char l5payload[PACKET_MTU];
    char *l5_ptr = NULL; 
    char * line = NULL;
    uint16_t l5_len = 0;
    char *method = NULL, *uri = NULL;    
    char *begin = NULL;

    l5_ptr = l5payload;
    l5_len = hytag->l5_len;

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
        pid_add_count(APP_HTTP_GET_BYTES,hytag->pbuf.len);
        uri = strsep(&line, " ");
        if(uri != NULL)
        {
            strncpy(hytag->uri, uri, URL_MAX_LEN);
            hytag->uri_len = strlen(uri);
            hytag->app_type = APP_TYPE_HTTP_GET_OR_POST;
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

	int len = 0;
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
            }			
		}
	}

    if(hytag->uri[0] == '/' && hytag->host_len > 0 && hytag->uri_len > 0)
    {
        hytag->url_len= snprintf(hytag->url, URL_MAX_LEN, "%s%s",
                                                hytag->host, hytag->uri);
    }

    return E_SUCCESS;
}

