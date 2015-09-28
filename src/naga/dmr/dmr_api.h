
#ifndef __DMR_API_H__
#define __DMR_API_H__


#include "naga_host_rule.h"


berr  dmr_api_rule_host_add(naga_host_rule_t *entry);


berr  dmr_api_rule_host_del(naga_host_rule_t *entry);



naga_host_rule_t *dmr_api_rule_host_show(naga_host_rule_t *data);

berr dmr_api_rule_host_del_all(void);




#endif
