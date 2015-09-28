/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/home/shan/work/rose/src/test-1/template\packet_http.h
#       @date         :2015/09/23 08:48
#       @algorithm    :
=============================================================================*/

#ifndef __PACKET_HTTP_INCLUDE_H__
#define __PACKET_HTTP_INCLUDE_H__
#include "pid.h"
berr ads_http_content_fill(char *buf, hytag_t *hytag);
berr ads_http_ok_head_fill(char *buf, hytag_t *hytag);
berr ads_template_init(void);
#endif /* end of __PACKET_HTTP_INCLUDE_H__ */
