#ifndef __UPUSH_H__
#define __UPUSH_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "boots.h"
#include "naga_types.h"
#include "bts_list.h"

berr upush_wb_enable_set(int status);
berr upush_rule_add(char *u_str);
berr upush_rule_del(void);
char * upush_rule_get(void);
berr upush_send(char *string);
berr upush_rule_test(void);
berr upush_content_generator(hytag_t *hytag, char *ptr);
#endif /* !__UPUSH_H__ */
