#include "pid.h"
#include "http_parser.h"
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


#if 0
static inline berr pid_http_request_host(uint8_t *p ,  uint8_t *host, uint16_t *len)
{
	int i = 0;
	while(('\r' != *p) && ('\0' != *p))    /*GET*/
	{
		if (i + 1 > MAX_HOST_LEN)
		{
			//pid_incr_count(HTTP_URL_EXCEED);
			BRET(E_FAIL);
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
        BRET(E_PARAM);
    return E_SUCCESS;
}
#if 0

berr pid_http_up(struct pbuf *p ,  hytag_t * hytag )
{
	uint16_t method_len = 0, vsr_len = 0;
	uint16_t len = 0;
	uint8_t *http_p = NULL;
	uint8_t method_http[MAX_METHOD_LEN] = {0};
	char *line = NULL, *begin = NULL, *context_p = NULL;
	
	PBUF_CUR_FORMAT(uint8_t *, http_p, p);

    hytag->app_type = APP_TYPE_HTTP_OTHER; 

	if(!pid_http_request_method(http_p, method_http, &method_len))
	{
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

	if (pid_http_request_url(http_p, (uint8_t *)hytag->uri, &hytag->uri_len))
	{
		//hytag->app_type = URL_IN_NULL;
		return E_SUCCESS;
	}
	else
	{
		hytag->app_type = APP_TYPE_HTTP_GET_OR_POST;
	}
    	//printf("the url is:  %s\n", hytag->url);



	

	len = hytag->uri_len + 1;
	UPDATE_PBUF_OFFSET(p, len);
	PBUF_CUR_FORMAT(uint8_t *, http_p, p);

	if(pid_http_request_version_skip(http_p, &vsr_len))
	{
		return E_SUCCESS;
	}

	len = vsr_len + 1;
	UPDATE_PBUF_OFFSET(p, len);
#if 1
	PBUF_CUR_FORMAT(char *, context_p, p);

	char http_buf[1500];
	char *http_bak=http_buf;
        
	char line_buf[1500];
	char *line_bak=line_buf;
	
	memcpy(http_bak, context_p, p->len-p->ptr_offset);
	http_bak[p->len - p->ptr_offset+1] = '\0';	
			
	
	while(NULL != (line = strsep(&http_bak, "\n")))
	{

		line_bak= line_buf;
		strcpy(line_bak, line);
		begin = strsep(&line_bak, ":");
		if(  NULL != line_bak && NULL != begin )
		{	
		#if 0
			int len =  strlen(line_bak);
			if(len <= 2)
			{
				//printf("line = %s<%s>\n", line_bak, context_p);
				continue;
			}	
			if (hytag->host_len ==0 
					&& !strncasecmp(STRING_HTTP_HOST, begin, STRING_HTTP_HOST_LEN)) 
			{

				memcpy(hytag->host, &line_bak[1], (len-2));
				hytag->host_len = len -2;
			}
			if (hytag->user_agent_len== 0 
					&& !strncasecmp(STRING_HTTP_AGENT, begin, STRING_HTTP_AGENT_LEN))   
			{

				memcpy(hytag->user_agent, &line_bak[1], (len-2));
				hytag->user_agent_len = len-2;
			}				
		#endif
		}

	}


#endif
	/*check The First char*/
	if(hytag->uri[0] == '/')
	{
		hytag->url_len= snprintf(hytag->url, 256, "http://%s%s",hytag->host, hytag->uri);
	}
	return E_SUCCESS;
}




#else






#if 0
#define URL_HOST_MAX 128
#define URL_URI_MAX 1500
#define URL_PCNT_MAX 32
#define URL_PKEY_MAX

typedef struct {
    char host[URL_HOST_MAX];
    char uri[URL_URI_MAX];
    int pcnt;
    char pkey[URL_PCNT_MAX][URL_PKEY_MAX];
    char pval[URL_PCNT_MAX][URL_PVAL_MAX];
} url_t;

static berr pid_url(const char *msg, char *urlstr)
{
    char *p1 = NULL;
    char *p = urlstr;
    int len = 0;

    if (p == NULL) {
        return E_FAIL;
    }

    url_t url;

    memset((char *) &url, 0, sizeof(url_t));

    printf("%s:%d: %s, url=%s\n", __func__, __LINE__, msg, urlstr);
    

    if (strstr(p, "http:/")) {
        p += strlen("http://");

        p1 = strstr(p, "/");

        if (p1) {
            len = p1 - p;

            if (len > (URL_HOST_MAX - 1)) {
                len = URL_HOST_MAX - 1;
            }

            strncpy(url.host, p, len);  
            p += len;
        }

    }

    if (p[0] != '/') {
        return E_FAIL;
    }

    p += 1;

    
    p1 = strstr(p, "?");

    if (p1) {
        len = p1 - p;
    } else {
        len = strlen(p);
    }

    if (len > (URL_URI_MAX - 1)) {
        len = URL_URI_MAX - 1;
    }

    strncpy(url.uri, p, len); 
    p += len;

    printf("HOST: %s\n", url.host);
    printf("URI: %s\n", url.uri);


    return E_SUCCESS;
}
#else

#define DUMP_URL 0
#if DUMP_URL
static void dump_url (const char *url, const struct http_parser_url *u)
{
  unsigned int i;

  printf("--------------------------------------------------------------------\n");
  printf("url=%s\n", url);
  printf("\tfield_set: 0x%x, port: %u\n", u->field_set, u->port);
  for (i = 0; i < UF_MAX; i++) {
    if ((u->field_set & (1 << i)) == 0) {
      printf("\tfield_data[%u]: unset\n", i);
      continue;
    }

    printf("\tfield_data[%u]: off: %u, len: %u, part: %.*s\n",
           i,
           u->field_data[i].off,
           u->field_data[i].len,
           u->field_data[i].len,
           url + u->field_data[i].off);
  }
}
#endif

#if HTTP_URL_PARSE_ORI_MOD
static berr pid_url_param(char *str, naga_url_param_t *param)
{
    char *token, *peq = NULL;

    //printf("param=%p, str =%s\n", param, str);

    if (NULL == str || NULL == param) {
        return E_FAIL; 
    }

    token = strtok(str, "&");
    while(token !=NULL)
    {
       
        debug("token %d: %s", param->pnumb, token);
        
        if (param->pnumb < URL_PARAM_NUMB_MAX) {
            peq = strstr(token, "=");

            if (peq) {
                strncpy(param->pkeys[param->pnumb], token, peq - token);
                strcpy(param->pvals[param->pnumb], peq + 1);
                param->pnumb += 1;
            }

            token=strtok(NULL, "&");
        }       
        else
        {
            //param->pnumb > 32
            break;
        }
    }

    return E_SUCCESS;
} 


static berr pid_url(char *str, int len, naga_url_t *nu)
{
    struct http_parser_url u;

    int result;

    http_parser_url_init(&u);


    result = http_parser_parse_url(str, len, 0, &u);

    if (result != 0) {
        pid_incr_count(APP_HTTP_PARSE_ERR);
        return E_FAIL; 
    }

    #if DUMP_URL
    dump_url(str, &u);
    #endif

    if (u.field_data[1].len > 0) {
        memcpy(nu->host, str + u.field_data[1].off, u.field_data[1].len);
    }

    if (u.field_data[3].len > 0) {
        memcpy(nu->uri, str + u.field_data[3].off, u.field_data[3].len);
    }

    if (u.field_data[4].len > 0) {
        memcpy(nu->pstr, str + u.field_data[4].off, u.field_data[4].len);
        pid_url_param(str + u.field_data[4].off, &nu->params);
    }

    return E_SUCCESS;
}
#endif
#endif

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
#if HTTP_URL_PARSE_ORI_MOD
            len = strlen(uri);
            if( len > URL_URI_LEN_MAX )
            {	
                len = URL_URI_LEN_MAX ;
            }

            debug("get uri:%s", uri);
            debug("get uri len:%d", len);
            pid_url(uri, len, &hytag->ori_url);
            debug("get uri over");
#else
            strncpy(hytag->uri, uri, URL_MAX_LEN);
            hytag->uri_len = strlen(uri);
#endif
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
#if HTTP_URL_PARSE_ORI_MOD
            if (hytag->ori_url.host_len ==0 
                && !strncmp(STRING_HTTP_HOST, begin, STRING_HTTP_HOST_LEN)) 
			{
               
                if( len > URL_HOST_LEN_MAX )
				{	
					len = URL_HOST_LEN_MAX-1;
				}
           
				memcpy(hytag->ori_url.host, &line[1], len);
				hytag->ori_url.host_len = len;
                debug("host(%d):%s", hytag->ori_url.host_len,hytag->ori_url.host);
               
			}
#else
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
#endif

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
#if HTTP_URL_PARSE_ORI_MOD
            if (!strncmp(STRING_HTTP_REFERENCE, begin, STRING_HTTP_REFERENCE_LEN))
            {
                if( len > URL_URI_LEN_MAX )
                {	
                    len = URL_URI_LEN_MAX-1;
                }
                debug("get refer");
                pid_url(&line[1], len, &hytag->ref_url);
                debug("get refer over");
            }
#else
            if (hytag->referer_len== 0 
                    && !strncmp(STRING_HTTP_REFERENCE, begin, STRING_HTTP_REFERENCE_LEN))
            {

                if( len > URL_MAX_LEN )
                {	
                    len = URL_MAX_LEN;
                }
                memcpy(hytag->referer, &line[1], len) ; 
                hytag->referer_len = len; 
                //pid_url("REF", hytag->referer, hytag->referer_len);
            }
#endif
		
		}
	}
#if HTTP_URL_PARSE_ORI_MOD
    if (hytag->ori_url.pstr[0] != 0) {
        sprintf(hytag->ori_url.url, "%s%s?%s", hytag->ori_url.host, hytag->ori_url.uri, hytag->ori_url.pstr); 
    } else {
        sprintf(hytag->ori_url.url, "%s%s", hytag->ori_url.host, hytag->ori_url.uri);
    }

    if (hytag->ref_url.pstr[0] != 0) {
        sprintf(hytag->ref_url.url, "%s%s?%s", hytag->ref_url.host, hytag->ref_url.uri, hytag->ref_url.pstr); 
    } else {
        sprintf(hytag->ref_url.url, "%s%s", hytag->ref_url.host, hytag->ref_url.uri);
    }

    if (DOPT_IS_SELECTED(DOPT_DUMP_URL)) 
    {
        printf("\nDOPT: dump_url %14s.%-6d ===================================================================\n", __func__, __LINE__);
        printf("ORI_URL:\n");
        naga_url_dump(&hytag->ori_url);
        printf("\nREF_URL:\n");
        naga_url_dump(&hytag->ref_url);
        printf("--------------------------------------------------------------------------------------------------------\n");
    }
#else
    if(hytag->uri[0] == '/' && hytag->host_len > 0 && hytag->uri_len > 0)
    {
        hytag->url_len= snprintf(hytag->url, URL_MAX_LEN, "%s%s",
                hytag->host, hytag->uri);
    }
#endif

    return E_SUCCESS;
}



#endif






#if 0
berr pid_http_get(struct pbuf *p,  hytag_t * hytag)
	    irintf("")	
{

       
}

berr pid_http_post(struct pbuf *p,  hytag_t * hytag)
{

       
}
#endif
