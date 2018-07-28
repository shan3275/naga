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

#include "ua.h"
#include "ua_cmd.h"

DEFUN(ua_add,
      ua_add_cmd,
      "ua add <0-15> EXPR ACT",
      UA_EXPR
      ADD_EXPR
      UA_INDEX_EXPR
      UA_PCRE_EXPR
      UA_ACT_EXPR)
{

    char url_str[1024];
    int  url_chr_index = 0;
    uint32_t action;
    int index = strtoul(argv[0], NULL, 0 );
	berr rv;
	//char *straction = strdup(argv[2]);
    char * exprstr =  strdup(argv[1]);
    char  lastchar = '\0';
    int i;
    int len = strlen(exprstr);
    naga_acl_t acl;


    memset(&acl, 0, sizeof(naga_acl_t));

	if(naga_acl_parse(&argv[2], argc - 2, &acl))
    {
    	//free(straction);
        return CMD_ERR_NO_MATCH;
    }
    //free(straction);


    for(i=0; i<len; i++)
    {
        
        switch(exprstr[i])
        {
            case 'Q':
                if(lastchar == '\\')
                {
                   url_str[url_chr_index-1] = '?' ;                                                                                            
                }
                break;
            default:
                url_str[url_chr_index++] = exprstr[i];
                break;
        }
        lastchar = exprstr[i];
    }
    url_str[url_chr_index]  = '\0';     
    
    rv =  ua_rule_add(index, url_str , exprstr, &acl);
    free(exprstr);
	if(rv != E_SUCCESS)
	{
		 vty_out(vty, "Failed To add Url rule %s", VTY_NEWLINE);
		 return 0;
	}
	return 0;
}

ALIAS (ua_add,
       ua_add_param_cmd,
       "ua add <0-15> EXPR ACT PARAM",
      UA_EXPR
      ADD_EXPR
      UA_INDEX_EXPR
      UA_PCRE_EXPR
      UA_ACT_EXPR
      UA_PARAM_EXPR)

ALIAS (ua_add,
       ua_add_param_rate_cmd,
       "ua add <0-15> EXPR ACT PARAM <1-100>",
      UA_EXPR
      ADD_EXPR
      UA_INDEX_EXPR
      UA_PCRE_EXPR
      UA_ACT_EXPR
      UA_PARAM_EXPR
      UA_RATE_EXPR)

DEFUN(ua_del,
      ua_del_cmd,
      "ua del <0-15>",
      UA_EXPR
      DEL_EXPR
      UA_INDEX_EXPR)
{
    int index = strtoul(argv[0], NULL, 0 );
    return ua_rule_del(index);
}


DEFUN(ua_del_all,
      ua_del_all_cmd,
      "ua del all",
      UA_EXPR
      DEL_EXPR
      UA_ALL_EXPR)
{
    int i;
    for(i=0; i <MAX_UA_RULE; i++)
    {
        ua_rule_del(i);
    }
    return 0;
}

DEFUN(show_ua_all,
      show_ua_all_cmd,
      "show ua all",
      SHOW_UA_EXPR
      UA_EXPR
      UA_ALL_EXPR)
{

    struct pcre_s *pcreptr = NULL;
    int i;
    char acl_str[NAGA_ACL_STR_SZ];

    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    
    vty_out(vty, "%-6s %-16s %-16s %-16s %-6s %-64s %s %s", "ID", "cnt", "drop", "push", "rate", "URL", "action", VTY_NEWLINE);
    for(i=0; i < MAX_UA_RULE; i++)
    {
        pcreptr = ua_rule_get(i);
        if(!pcreptr)
        {
            continue;

        }
		if(pcreptr->used)
		{
            naga_acl_string(&pcreptr->acl, acl_str);
        	vty_out(vty, "%-6d %-16ld %-16ld %-16ld %-6u %-64s %s %s", pcreptr->id, (uint64_t) pcreptr->acl.cnt.cnt, 
                    (uint64_t) pcreptr->acl.vcnt.cnt, (uint64_t) pcreptr->acl.pushed_cnt.cnt, 
                    pcreptr->acl.rate ,pcreptr->pattern, acl_str, VTY_NEWLINE);    
		}

    }
    return 0;
}


void ua_cmd_config_write(struct vty *vty)
{
    struct pcre_s *pcreptr = NULL;
    int i;

    char acl_str[NAGA_ACL_STR_SZ];
    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    for(i=0; i < MAX_UA_RULE; i++)
    {
        pcreptr = ua_rule_get(i);
        if(!pcreptr)
            continue;
        if(pcreptr->used)
        {
            naga_acl_string(&pcreptr->acl, acl_str);

            vty_out(vty, "ua add %d %s %s %s", pcreptr->id, pcreptr->cli_pattern, acl_str,  VTY_NEWLINE);    
        }
    }
    return ;
}

void cmdline_ua_init(void)
{
    install_element(CMD_NODE, &ua_add_cmd);
    install_element(CMD_NODE, &ua_add_param_cmd);
    install_element(CMD_NODE, &ua_add_param_rate_cmd);
    install_element(CMD_NODE, &ua_del_cmd);
    install_element(CMD_NODE, &ua_del_all_cmd);
    install_element(CMD_NODE, &show_ua_all_cmd);

    return ;
}
