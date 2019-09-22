#ifndef __REDISPUSH_H__
#define __REDISPUSH_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "boots.h"
#include "naga_types.h"
#include "bts_list.h"

berr redispush_rule_add(char *s_str, char *p_str);
berr redispush_rule_del(void);
berr redispush_rule_get(uint32_t *sip, uint16_t *dport);
berr redispush_rule_test(char * buff);
berr RedisStore(uint8_t status, char * ucIp, char* ucUserName);

#endif