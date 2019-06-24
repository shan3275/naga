/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-07-28 06:03
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#ifndef __RAW_UDP_H__
#define __RAW_UDP_H__
#include "naga_types.h"
berr raw_udp_content_generator(void *buffer, char * payload, int *len, hytag_t *hytag);
berr raw_udp_test(void);
#endif
