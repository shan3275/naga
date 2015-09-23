/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/home/shan/work/rose/src/test-1/template\packet_http.c
#       @date         :2015/09/19 18:30
#       @algorithm    :
=============================================================================*/

// http get header
//*****************************************************************************
//  sample code:
//  GET /test.html HTTP/1.1
//  Host: vlinux
//  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//  Accept-Language: zh-cn
//  Connection: keep-alive
//  Accept-Encoding: gzip, deflate
//  User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_3) AppleWebKit/600.6.3 (KHTML, like Gecko) Version/8.0.6 Safari/600.6.3
// * ***************************************************************************/
//
// ascii code:
// HTTP_GET "/test.html" HTTP_ONE_SPACE HTTP_VERSION HTTP_CRLF
// HTTP_HOST_HEAD "www.bai.com" HTTP_CRLF
// HTTP_ACCEPT HTTP_CRLF
// HTTP_ACCEPT_LANGUAGE HTTP_CRLF
// HTTP_CONNECTION HTTP_CRLF
// HTTP_ACCEPT_ENCODING HTTP_CRLF
// HTTP_USER_AGENT HTTP_CRLF
// HTTP_CRLF
//*****************************************************************************
#include "boots.h"
#include "packet_http.h"
#include "rte_memcpy.h"

#define HTTP_ONE_SPACE " "
#define HTTP_FOUR_SPACE HTTP_ONE_SPACE HTTP_ONE_SPACE HTTP_ONE_SPACE HTTP_ONE_SPACE
#define HTTP_CRLF                   "\r\n"
#define HTTP_GET                    "GET "
#define HTTP_VERSION                "HTTP/1.1"
#define HTTP_HOST_HEAD              "Host: "
#define HTTP_ACCEPT                 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
#define HTTP_ACCEPT_LANGUAGE        "Accept-Language: zh-cn"
#define HTTP_CONNECTION             "Connection: keep-alive"
#define HTTP_ACCEPT_ENCODING        "Accept-Encoding: gzip, deflate"
#define HTTP_USER_AGENT             "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_3) AppleWebKit/600.6.3 (KHTML, like Gecko) Version/8.0.6 Safari/600.6.3"


#define HTTP_RESPONSE_OK            "200 OK"
#define HTTP_KEEP_ALIVE             "Keep-Alive: timeout=60, max=199"
#define HTTP_CONTENT_TYPE           "Content-Type: text/html"
#define HTTP_SERVER                 "Server: Embedthis-http"
#define HTTP_DATA_HEAD              "Date: "
#define HTTP_ETAG                   "ETag: 60c03-521-55f6e127"
#define HTTP_CONTENT_LENGTH_HEAD    "Content-Length: "
#define HTTP_LAST_MODIFIED_HEAD     "Last-Modified: "
#define HTTP_ACCEPT_RANGES          "Accept-Ranges: bytes"

// http response
//*****************************************************************************
// sample code :
// HTTP/1.1 200 OK
// Keep-Alive: timeout=60, max=199
// Content-Type: text/html
// Server: Embedthis-http
// Date: Mon, 14 Sep 2015 15:02:34 GMT
// ETag: "60c03-521-55f6e127"
// Content-Length: 1313
// Last-Modified: Mon, 14 Sep 2015 15:00:55 GMT
// Connection: Keep-Alive
// Accept-Ranges: bytes
// 
// <!DOCTYPE HTML>
// <html>
//     <head>
//         <meta charset="utf-8">
//         <title></title>
//         <script>
//             d=document;
//             function u(){
//             var f = "http://www.taobao.com";
//             d.getElementById("m").src=f+(f.indexOf("&")<0?'?':'&')+'_t=t';
//             }
// 
//             setTimeout(function(){d.getElementById("x").style.display='block';}, 2000);     
// 
//             function c(){
//             x.style.display="none"
//             }
// 
//         </script>
//         <style>
//             body {margin:0;color:#000;overflow:hidden;padding:0;height:100%;font-family:Arial}
//             a{cursor:pointer;display:block;position:absolute;border:1px;border-radius:1em;background-color:#555;color:#eee;z-index:3;right:5px;top:5px;line-height:20px;text-align:center;width:20px;font-size:10px}
//             #x{position:absolute;z-index:2;right:18px;bottom:0px;width:300px;height:250px}
//             #i{display:block; position:absolute; z-index:1; width:100%; height:100%}
//         </style>
//     </head>
//     <body onLoad=u()>
//         <div id=i>
//             <iframe id=m frameborder=0 width=100% height=100%></iframe>
//         </div>
//         <div id=x>
//             <iframe src="http://www.121zou.com/speed.html" width=300 height=250 scrolling=no frameborder=0></iframe>
//         </div>
// 
//     </body>
// </html>
//********************************************************************************
//
// response head acscii code:
// HTTP_VERSION HTTP_ONE_SPACE HTTP_RESPONSE_OK HTTP_CRLF
// HTTP_KEEP_ALIVE HTTP_CRLF
// HTTP_CONTENT_TYPE HTTP_CRLF
// HTTP_SERVER HTTP_CRLF
// HTTP_DATA_HEAD "Mon, 14 Sep 2015 15:02:34 GMT" HTTP_CRLF
// HTTP_ETAG HTTP_CRLF
// HTTP_CONTENT_LENGTH_HEAD "1313" HTTP_CRLF
// HTTP_LAST_MODIFIED_HEAD "Mon, 14 Sep 2015 15:00:55 GMT" HTTP_CRLF
// HTTP_CONNECTION HTTP_CRLF
// HTTP_ACCEPT_RANGES HTTP_CRLF
//********************************************************************************/


/* http get  head template 1*/
char *http_head_get1 = 
    HTTP_GET; 
char *http_head_get2 = "/test.html"; 
char *http_head_get3 = 
    HTTP_ONE_SPACE HTTP_VERSION HTTP_CRLF
    HTTP_HOST_HEAD; 
char *http_head_get4 = "www.bai.com"; 
char *http_head_get5 = 
    HTTP_CRLF
    HTTP_ACCEPT HTTP_CRLF
    HTTP_ACCEPT_LANGUAGE HTTP_CRLF
    HTTP_CONNECTION HTTP_CRLF
    HTTP_ACCEPT_ENCODING HTTP_CRLF
    HTTP_USER_AGENT HTTP_CRLF
    HTTP_CRLF;

/* http response ok template 1*/
char *http_head_response1=
    HTTP_VERSION HTTP_ONE_SPACE HTTP_RESPONSE_OK HTTP_CRLF
    HTTP_KEEP_ALIVE HTTP_CRLF
    HTTP_CONTENT_TYPE HTTP_CRLF
    HTTP_SERVER HTTP_CRLF
    HTTP_DATA_HEAD;

char *http_head_response2= "Mon, 14 Sep 2015 15:02:34 GMT";
char *http_head_response3=  \
    HTTP_CRLF \
    HTTP_ETAG  \
    HTTP_CRLF \
    HTTP_CONTENT_LENGTH_HEAD;
    
char *http_head_response4= "1313";
char *http_head_response5=
    HTTP_CRLF
    HTTP_LAST_MODIFIED_HEAD;
    
char *http_head_response6= "Mon, 14 Sep 2015 15:00:55 GMT"; 
char *http_head_response7=
    HTTP_CRLF
    HTTP_CONNECTION HTTP_CRLF
    HTTP_ACCEPT_RANGES HTTP_CRLF;



/*body template 1 */
char * http_body1 =
    "<!DOCTYPE HTML>\n"
    "<html>\n"
    "    <head>\n"
    "        <meta charset=\"utf-8\">"
    "        <title></title>\n"
    "        <script>\n"
    "            d=document;\n"
    "            function u(){\n"
    "            var f = \"";

char *http_body2 = "www.taobao.com";

char *http_body3 =
    "\";\n"
    "            d.getElementById(\"m\").src=f+(f.indexOf(\"&\")<0\?\'\?\':\'&\')+\'_t=t\';\n"
    "            }\n"
    "\n"
    "            setTimeout(function(){d.getElementById(\"x\").style.display=\'block\';}, 2000);\n"
    "\n"
    "            function c(){\n"
    "            x.style.display=\"none\"\n"
    "            }\n"
    "\n"
    "        </script>\n"
    "        <style>\n"
    "            body {margin:0;color:#000;overflow:hidden;padding:0;height:100%;font-family:Arial}\n"
    "            a{cursor:pointer;display:block;position:absolute;border:1px;border-radius:1em;background-color:#555;color:#eee;z-index:3;right:5px;top:5px;line-height:20px;text-align:center;width:20px;font-size:10px}\n"
    "            #x{position:absolute;z-index:2;right:18px;bottom:0px;width:300px;height:250px}\n"
    "            #i{display:block; position:absolute; z-index:1; width:100%; height:100%}\n"
    "        </style>\n"
    "    </head>\n"
    "    <body onLoad=u()>\n"
    "        <div id=i>\n"
    "            <iframe id=m frameborder=0 width=100% height=100%></iframe>\n" 
    "        </div>\n"
    "        <div id=x>\n"
    "            <iframe src=\"";

char *http_body4 = "http://www.121zou.com/speed.html";
char *http_body5 =
    "\" width=300 height=250 scrolling=no frameborder=0></iframe>\n"
    "        </div>\n"
    "\n"
    "    </body>\n"
    "</html>\n"
    "\n";





















uint16_t
http_content_len_get(hytag_t *hytag)
{
    uint16_t len = 0;
    len += strlen(http_body1);
    len += hytag->url_len;
    len += hytag->host_len;
    len += strlen(http_body3);
    len += strlen(http_body4);
    len += strlen(http_body5);

    return len;
}

berr ads_http_ok_head_fill(char *buf, hytag_t *hytag)
{
    uint16_t len = 0;
    uint16_t content_len = 0;
    char content_buff[10] = {0};

    if ( NULL == buf || NULL == hytag )
    {
        return E_PARAM;
    }

    rte_memcpy(buf + len, http_head_response1, (size_t) strlen(http_head_response1));
    len += strlen(http_head_response1);

    rte_memcpy(buf + len, http_head_response2, (size_t) strlen(http_head_response2));
    len += strlen(http_head_response2);

    rte_memcpy(buf + len, http_head_response3, (size_t) strlen(http_head_response3));
    len += strlen(http_head_response3);

    /* get content length */
    content_len = http_content_len_get(hytag);
    sprintf(content_buff, "%d", content_len);
    rte_memcpy(buf + len, content_buff, (size_t) strlen(content_buff));
    len += strlen(content_buff);


    rte_memcpy(buf + len, http_head_response5, (size_t) strlen(http_head_response5));
    len += strlen(http_head_response5);

    rte_memcpy(buf + len, http_head_response6, (size_t) strlen(http_head_response6));
    len += strlen(http_head_response6);

    rte_memcpy(buf + len, http_head_response7, (size_t) strlen(http_head_response7));
    len += strlen(http_head_response7);

    hytag->l5_len = len;
    return E_SUCCESS;
}


berr ads_http_content_fill(char *buf, hytag_t *hytag)
{
    uint16_t len = 0;
    uint16_t content_len = 0;
    char *content_buff[10] = {0};

    if ( NULL == buf || NULL == hytag )
    {
        return E_PARAM;
    }

    rte_memcpy(buf + len, http_body1, (size_t) strlen(http_body1));
    len += strlen(http_body1);

    rte_memcpy(buf + len, hytag->host, (size_t) hytag->host_len);
    len += hytag->host_len;

    rte_memcpy(buf + len, hytag->url, (size_t) hytag->url_len);
    len += hytag->url_len;

    rte_memcpy(buf + len, http_body3, (size_t) strlen(http_body3));
    len += strlen(http_body3);

    rte_memcpy(buf + len, http_body4, (size_t) strlen(http_body4));
    len += strlen(http_body4);

    rte_memcpy(buf + len, http_body5, (size_t) strlen(http_body5));
    len += strlen(http_body5);

    hytag->l5_len = len;
    return E_SUCCESS;
}

