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

DEFUN(url_add,
      url_add_cmd,
      "url add <0-63> EXPR ACT",
      URL_EXPR)
{

    char url_str[512];
    int  url_chr_index = 0;
    uint32_t action;
    int index = strtoul(argv[0], NULL, 0 );
	berr rv;
	char *straction = strdup(argv[2]);
    char * exprstr = argv[1];
    char  lastchar = '\0';
    int i;

    
	if(naga_action_parse(straction, &action))
    {
    	free(straction);
        return CMD_ERR_NO_MATCH;
    }
    free(straction);


    for(i=0; i<strlen(exprstr); i++)
    {
        
        switch(exprstr[i])
        {
            case 'Q':
                if(lastchar == '\\')
                {
                   url_str[url_chr_index-1] = '?'                                                                                             
                }
                break;
            default:
                url_str[url_chr_index++] = exprstr[i];
                break;
        }
    }
    url_str[url_chr_index]  = '\0';     
    
    rv =  url_rule_add(index, url_str , argv[1], action);
	if(rv != E_SUCCESS)
	{
		 vty_out(vty, "Failed To add Url rule %s", VTY_NEWLINE);
		 return 0;
	}
	return 0;
}

DEFUN(url_del,
      url_del_cmd,
      "url del <0-63>",
      URL_EXPR)
{


    int index = strtoul(argv[0], NULL, 0 );
    
    return url_rule_del(index);
}


DEFUN(url_del_all,
      url_del_all_cmd,
      "url del all",
      URL_EXPR)
{


    int i;
    for(i=0; i <MAX_URL_RULE; i++)
    {
        url_rule_del(i);
    }
    return 0;
}



DEFUN(show_url_all,
      show_url_all_cmd,
      "show url all",
      URL_EXPR)
{

    struct pcre_s *pcreptr = NULL;
    int i;

    vty_out(vty, "%-32s %-32s %-32s %-32s %s", "ID", "URI","URI-CLI" "cnt", VTY_NEWLINE);
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = url_rule_get(i);
        if(!pcreptr)
        {
            continue;

        }
		if(pcreptr->used)
        	vty_out(vty, "%-32d %-32s %-32s %-20ld %s", pcreptr->id, pcreptr->pattern, pcreptr->cli_pattern,(uint64_t) pcreptr->acl.cnt.cnt,  VTY_NEWLINE);    

        
    }
    return 0;
}




void url_cmd_config_write(struct vty *vty)
{

    struct pcre_s *pcreptr = NULL;
    int i;

    char action_str[NAGA_ACTION_STR_SZ];
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = url_rule_get(i);
        if(!pcreptr)
            continue;

        naga_action_string(&(pcreptr->acl.actions), action_str);

        vty_out(vty, "url add %d %s %s", pcreptr->id, pcreptr->cli_pattern, action_str,  VTY_NEWLINE);    
        
    }
    return ;
}


void cmdline_url_init(void)
{
	install_element(CMD_NODE, &url_add_cmd);
	install_element(CMD_NODE, &url_del_cmd);
	install_element(CMD_NODE, &url_del_all_cmd);
    install_element(CMD_NODE, &show_url_all_cmd);
    return ;
}




