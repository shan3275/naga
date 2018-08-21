#include <string.h>
#include <stdio.h>
#include "zebra.h"
#include "version.h"
#include "getopt.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"
#include "packet.h"
#include "naga_types.h"
#include "naga_util.h"

#include "bts_cnt.h"


#include "mask.h"
#include "mask_cmd.h"

DEFUN(mask_add,
      mask_add_cmd,
      "mask add <0-63> MASK ACT",
      MASK_EXPR)
{
    berr rv;
    uint32_t action;

    int index = strtoul(argv[0], NULL, 0);
	

    int mask =  strtoul(argv[1], NULL, 0);


    naga_acl_t acl;
    memset(&acl, 0, sizeof(naga_acl_t));

	if(naga_acl_parse(&argv[2], argc - 2, &acl))
    {

        return CMD_ERR_NO_MATCH;
    }
    
    rv =  mask_rule_add(index, mask, &acl);
 
	if(rv != E_SUCCESS)
	{
		 vty_out(vty, "Failed To add Url rule %s", VTY_NEWLINE);
		 return 0;
	}
	return 0;
}

ALIAS (mask_add,
       mask_add_param_cmd,
       "mask add <0-63> MASK ACT PARAM",
       MASK_EXPR)

ALIAS (mask_add,
       mask_add_param_rate_cmd,
       "mask add <0-63> MASK ACT PARAM <1-100>",
       MASK_EXPR)

DEFUN(mask_del,
      mask_del_cmd,
      "mask del <0-63>",
      MASK_EXPR)
{


    int index = strtoul(argv[0], NULL, 0 );
    
    return mask_rule_del(index);
}


DEFUN(mask_del_all,
      mask_del_all_cmd,
      "mask del all",
      MASK_EXPR)
{


    int i;
    for(i=0; i <MAX_MASK_RULE; i++)
    {
        mask_rule_del(i);
    }
    return 0;
}



DEFUN(show_mask_all,
      show_mask_all_cmd,
      "show mask all",
      MASK_EXPR)
{

    mask_rule_t *rule = NULL;
    int i;
    char acl_str[NAGA_ACL_STR_SZ];

    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    
    vty_out(vty, "%-6s %-16s %-16s %-16s %-6s %-10s %s%s", "ID", "CNT", "DROP", "PUSH", "RATE", "MASK",  "ACTIONS",   VTY_NEWLINE);
    for(i=0; i < MAX_MASK_RULE; i++)
    {
        rule = mask_rule_get(i);

		if(rule && rule->used)
		{
            naga_acl_string(&rule->acl, acl_str);
        	vty_out(vty, "%-6d %-16ld %-16ld %-16ld %-6u 0x%.8x %s%s", rule->id, (uint64_t) rule->acl.cnt.cnt, 
                    (uint64_t)rule->acl.vcnt.cnt, (uint64_t)rule->acl.pushed_cnt.cnt, 
                    rule->acl.rate, rule->mask, acl_str, VTY_NEWLINE);    
		}
        
    }
    return 0;
}

void mask_cmd_config_write(struct vty *vty)
{

    mask_rule_t *rule = NULL;
    int i;

    char acl_str[NAGA_ACL_STR_SZ];
    memset(acl_str, 0, NAGA_ACL_STR_SZ);

    for(i=0; i < MAX_MASK_RULE; i++)
    {
        rule = mask_rule_get(i);

		if(rule && rule->used)
		{
            naga_acl_string(&rule->acl, acl_str);
        	vty_out(vty, "%-6d 0x%.8x %s%s", rule->id, rule->mask, acl_str, VTY_NEWLINE);    
		}
    }
    return ;
}


void cmdline_mask_init(void)
{
	install_element(CMD_NODE, &mask_add_cmd);
    install_element(CMD_NODE, &mask_add_param_cmd);
    install_element(CMD_NODE, &mask_add_param_rate_cmd);
	install_element(CMD_NODE, &mask_del_cmd);
	install_element(CMD_NODE, &mask_del_all_cmd);
    install_element(CMD_NODE, &show_mask_all_cmd);

    return ;
}




