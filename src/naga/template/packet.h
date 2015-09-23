/*=============================================================================
#       COPYRIGHT NOTICE
#       Copyright (c) 2015
#       All rights reserved
#
#       @author       :shan
#       @name         :Sam Liu
#       @file         :/home/shan/work/rose/src/test-1/template\packet.h
#       @date         :2015/09/23 08:50
#       @algorithm    :
=============================================================================*/

#ifndef __PACKET_INCLUDE_H__
#define __PACKET_INCLUDE_H__
#include "pid.h"


berr
ads_response_head_generator(struct rte_mbuf *m, hytag_t *hytag);
berr
ads_response_content_generator(struct rte_mbuf *m, hytag_t *hytag);
#endif
