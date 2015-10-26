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

#include "bts_cnt.h"


#include "url.h"
#include "url_cmd.h"





DEFUN(url_add,
      url_add_cmd,
      "url add <0-63> EXPR ACT",
      URL_EXPR)
{

    uint32_t action;
    int index = stroul(argv[0], NULL, 0 );
    
	if(naga_action_parse((char *)argv[2], &action))
    {
        return CMD_ERR_NO_MATCH;
    }
    
    return url_rule_add(index, argv[1], action);
}

DEFUN(url_add,
      url_add_cmd,
      "url add <0-63> EXPR ACT",
      URL_EXPR)
{

    uint32_t action;
    int index = stroul(argv[0], NULL, 0 );
    
	if(naga_action_parse((char *)argv[2], &action))
    {
        return CMD_ERR_NO_MATCH;
    }
    
    return url_rule_add(index, argv[1], action);
}

DEFUN(url_del,
      url_del_cmd,
      "url del <0-63>",
      URL_EXPR)
{


    int index = stroul(argv[0], NULL, 0 );
    
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

    vty_out(vty, "%-32s %-32s %-32s %s", "ID", "URI", "cnt", VTY_NEWLINE);
    for(i=0; i < MAX_URL_RULE; i++)
    {
        pcreptr = url_rule_get(i);
        if(!pcreptr)
            continue;

        vty_out(vty, "%-32d %-32s %-20sd %s", pcreptr->id, pcreptr->pattern,(uint64_t) pcreptr->acl.cnt.cnt,  VTY_NEWLINE);    
        
    }
    return 0;
}


void cmdline_url_init(void)
{
	install_element(CMD_NODE, &url_add_cmd);
	install_element(CMD_NODE, &rul_del_cmd);
	install_element(CMD_NODE, &url_del_all_cmd);
    install_element(CMD_NODE, &show_url_all_cmd);
    return ;
}




