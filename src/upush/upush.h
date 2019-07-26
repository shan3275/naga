#ifndef __UPUSH_H__
#define __UPUSH_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "boots.h"
#include "naga_types.h"
#include "bts_list.h"

#define RPUSH_SIP_ENABLE  1
#define RPUSH_SIP_DISABLE 0

berr upush_rule_add(char *u_str);
berr upush_rule_del(void);
char * upush_rule_get(void);
berr rpush_sip_enable_set(const char *en_str);
berr rpush_rule_sip_add(char *s_str);
berr upush_send(char *string);
berr upush_rule_test(void);
berr upush_content_generator(hytag_t *hytag, char *ptr);
berr wb_upush_content_generator(hytag_t *hytag, char *ptr);
#endif /* !__UPUSH_H__ */
