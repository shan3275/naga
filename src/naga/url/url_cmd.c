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


#include "url.h"
#include "url_cmd.h"

DEFUN(ori_url_add,
      ori_url_add_cmd,
      "ori url add <0-255> EXPR ACT",
      ORL_EXPR
      URL_EXPR
      ADD_EXPR
      URL_INDEX_EXPR
      URL_PCRE_EXPR
      URL_ACT_EXPR)
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
    
    rv =  ori_url_rule_add(index, url_str , exprstr, &acl);
    free(exprstr);
	if(rv != E_SUCCESS)
	{
		 vty_out(vty, "Failed To add Url rule %s", VTY_NEWLINE);
		 return 0;
	}
	return 0;
}

ALIAS (ori_url_add,
       ori_url_add_param_cmd,
       "ori url add <0-255> EXPR ACT PARAM",
       ORL_EXPR
       URL_EXPR
       ADD_EXPR
       URL_INDEX_EXPR
       URL_PCRE_EXPR
       URL_ACT_EXPR
       URL_PARAM_EXPR)

ALIAS (ori_url_add,
       ori_url_add_param_rate_cmd,
       "ori url add <0-255> EXPR ACT PARAM <1-100>",
       ORL_EXPR
       URL_EXPR
       ADD_EXPR
       URL_INDEX_EXPR
       URL_PCRE_EXPR
       URL_ACT_EXPR
       URL_PARAM_EXPR
       URL_RATE_EXPR)

DEFUN(ori_url_del,
      ori_url_del_cmd,
      "ori url del <0-255>",
      ORL_EXPR
      URL_EXPR
      DEL_EXPR
      URL_INDEX_EXPR)
{


    int index = strtoul(argv[0], NULL, 0 );
    
    return ori_url_rule_del(index);
}


DEFUN(ori_url_del_all,
      ori_url_del_all_cmd,
      "ori url del all",
      ORL_EXPR
      URL_EXPR
      DEL_EXPR
      URL_INDEX_EXPR)
{


    int i;
    for(i=0; i <MAX_URL_RULE; i++)
    {
        ori_url_rule_del(i);
    }
    return 0;
}



DEFUN(show_ori_url_all,
      show_ori_url_all_cmd,
      "show ori url all",
      URL_EXPR)
{

    struct pcre_s *pcreptr = NULL;
    int i;
    char acl_str[NAGA_ACL_STR_SZ];

    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    
    vty_out(vty, "%-6s %-16s %-16s %-16s %-6s %-64s %s %s", "ID", "cnt", "drop", "push", "rate", "URL", "action", VTY_NEWLINE);
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = ori_url_rule_get(i);
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

void ori_url_cmd_config_write(struct vty *vty)
{

    struct pcre_s *pcreptr = NULL;
    int i;

    char acl_str[NAGA_ACL_STR_SZ];
    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = ori_url_rule_get(i);
        if(!pcreptr)
            continue;
        if(pcreptr->used)
        {
            naga_acl_string(&pcreptr->acl, acl_str);

            vty_out(vty, "ori url add %d %s %s %s", pcreptr->id, pcreptr->cli_pattern, acl_str,  VTY_NEWLINE);    
        } 
    }
    return ;
}


DEFUN(ref_url_add,
      ref_url_add_cmd,
      "ref url add <0-255> EXPR ACT",
      URL_EXPR)
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
    
    rv =  ref_url_rule_add(index, url_str , exprstr, &acl);
    free(exprstr);
	if(rv != E_SUCCESS)
	{
		 vty_out(vty, "Failed To add Url rule %s", VTY_NEWLINE);
		 return 0;
	}
	return 0;
}

ALIAS (ref_url_add,
       ref_url_add_param_cmd,
       "ref url add <0-255> EXPR ACT PARAM",
       URL_EXPR)

ALIAS (ref_url_add,
       ref_url_add_param_rate_cmd,
       "ref url add <0-255> EXPR ACT PARAM <1-100>",
       URL_EXPR)

DEFUN(ref_url_del,
      ref_url_del_cmd,
      "ref url del <0-255>",
      URL_EXPR)
{


    int index = strtoul(argv[0], NULL, 0 );
    
    return ref_url_rule_del(index);
}


DEFUN(ref_url_del_all,
      ref_url_del_all_cmd,
      "ref url del all",
      URL_EXPR)
{
    int i;
    for(i=0; i <MAX_URL_RULE; i++)
    {
        ref_url_rule_del(i);
    }
    return 0;
}

DEFUN(show_ref_url_all,
      show_ref_url_all_cmd,
      "show ref url all",
      URL_EXPR)
{

    struct pcre_s *pcreptr = NULL;
    int i;
    char acl_str[NAGA_ACL_STR_SZ];

    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    
    vty_out(vty, "%-6s %-16s %-16s %-16s %-6s %-64s %s %s", "ID", "cnt", "drop", "push", "rate", "URL", "action", VTY_NEWLINE);
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = ref_url_rule_get(i);
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


void ref_url_cmd_config_write(struct vty *vty)
{

    struct pcre_s *pcreptr = NULL;
    int i;

    char acl_str[NAGA_ACL_STR_SZ];
    memset(acl_str, 0, NAGA_ACL_STR_SZ);
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = ref_url_rule_get(i);
        if(!pcreptr)
            continue;
        if(pcreptr->used)
        {
            naga_acl_string(&pcreptr->acl, acl_str);

            vty_out(vty, "ref url add %d %s %s %s", pcreptr->id, pcreptr->cli_pattern, acl_str,  VTY_NEWLINE);    
        } 
    }
    return ;
}

void cmdline_url_init(void)
{
	install_element(CMD_NODE, &ori_url_add_cmd);
    install_element(CMD_NODE, &ori_url_add_param_cmd);
    install_element(CMD_NODE, &ori_url_add_param_rate_cmd);
	install_element(CMD_NODE, &ori_url_del_cmd);
	install_element(CMD_NODE, &ori_url_del_all_cmd);
    install_element(CMD_NODE, &show_ori_url_all_cmd);

	install_element(CMD_NODE, &ref_url_add_cmd);
    install_element(CMD_NODE, &ref_url_add_param_cmd);
    install_element(CMD_NODE, &ref_url_add_param_rate_cmd);
	install_element(CMD_NODE, &ref_url_del_cmd);
	install_element(CMD_NODE, &ref_url_del_all_cmd);
    install_element(CMD_NODE, &show_ref_url_all_cmd);

    return ;
}
