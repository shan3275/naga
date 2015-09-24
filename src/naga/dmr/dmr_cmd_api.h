#ifndef __DMR_CMD_API_H__
#define __DMR_CMD_API_H__

#include "naga_host_rule.h"

berr rule_dmr_cmd_add_host(naga_host_rule_t *entry);


berr rule_dmr_cmd_del_host(naga_host_rule_t *entry);


naga_host_rule_t *rule_dmr_cmd_show_host(naga_host_attr_t *host);









#endif
