#include <string.h>
#include "boots.h"
#include "bts_debug.h"

#include "naga_types.h"
#include "naga_util.h"



berr 
naga_action_parse(char *str, uint32_t *actions)
{
    char *tokp = NULL; 
	char *act_str = NULL;

    if ((NULL == str) || (NULL == actions))
    {
        BRET(E_PARAM);
    }

    *actions = 0;

	act_str = str;

    tokp = strtok(act_str, ",");

    while(tokp != NULL) {
        
        if (!strcmp("log", tokp))
        {
            *actions |= ACT_LOG;
        }
        else if (!strcmp("trace", tokp))
        {
            *actions |= ACT_TRACE;
        }
        else if (!strcmp("drop", tokp))
        {
            *actions |= ACT_DROP;
        }
        else if (!strcmp("push", tokp))
        {
            *actions |= ACT_PUSH;
        }
        else if (!strcmp("redir", tokp))
        {
            *actions |= ACT_REDIR;
        }
        else if (!strcmp("tagdump", tokp))
        {
            *actions |= ACT_TAGDUMP;
        }
        else if (!strcmp("mask", tokp))
        {
            *actions |= ACT_MASK;
        }
        else if (!strcmp("upush", tokp))
        {
            *actions |= ACT_URLPUSH;
        }
        else
        {
            BRET(E_FOUND);
        }

        tokp = strtok(NULL, ","); 
    }

	return E_SUCCESS;
}


void 
naga_acl_string(naga_acl_t *acl, char *str)
{
    char *cp = NULL;

    if ((NULL == str) || (NULL == acl))
    {
        return ;
    }

    cp = str;

    if (ACT_IS_VAILD(acl->actions, ACT_LOG))
    {
    	if (cp != str)
        {
            cp += sprintf(cp, ",");
        }
        cp += sprintf(cp, "log");
    }

    if (ACT_IS_VAILD(acl->actions, ACT_TRACE))
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "trace");
    }

    if (ACT_IS_VAILD(acl->actions, ACT_DROP))
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "drop");
    }

    if (ACT_IS_VAILD(acl->actions, ACT_PUSH))
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "push");
    }

    if (ACT_IS_VAILD(acl->actions, ACT_URLPUSH))
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "upush");
    }

    if (ACT_IS_VAILD(acl->actions, ACT_TAGDUMP))
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "tagdump");
    }

    if (ACT_IS_VAILD(acl->actions, ACT_MASK)) 
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "mask");

        cp += sprintf(cp, " 0x%x", acl->mask);
    }

    if (ACT_IS_VAILD(acl->actions, ACT_REDIR)) 
    {
        if (cp != str)
        {
            cp += sprintf(cp, ",");
        }

        cp += sprintf(cp, "redir"); 

        cp += sprintf(cp, " %s", acl->url);

        cp += sprintf(cp, " %d", acl->rate);
    }
}

berr 
naga_acl_parse(const char *argv[], int argc, naga_acl_t *acl)
{
    berr rv = E_MAX;
    char *tokp = NULL;
	char *act_str = NULL, *mask_str = NULL;

    if ((NULL == argv) || (NULL == acl || 0 > argc || 3 < argc))
    {
        BRET(E_PARAM);
    }

    act_str = strdup(argv[0]);
    rv = naga_action_parse(act_str, &acl->actions); 

    if(E_SUCCESS != rv)
    {
        return rv;
    }
	
    if (acl->actions & ACT_MASK) {
        if (argc != 2) {
            BRET(E_PARAM);
        }
        mask_str = strdup(argv[1]);
        acl->mask = strtoul(argv[1], NULL, 0);
        if (0 == acl->mask) {
            BRET(E_PARAM);
        }

    }

    if (acl->actions & ACT_REDIR) {
        if (argc != 3) {
            BRET(E_PARAM);
        }

        if (NAGA_URL_LEN_MAX <= strlen(argv[1])) {
            BRET(E_PARAM);
        }

        strcpy(acl->url, argv[1]);
        acl->url[NAGA_URL_LEN_MAX - 1] = 0;

        acl->rate = strtoul(argv[2], NULL, 0);
        acl->sample = 100 / acl->rate;
    }

	return E_SUCCESS;
}

void naga_url_dump(naga_url_t *url)
{
    uint32_t i;

    if (NULL == url) {
        return;
    }
    printf("  URL : %s\n", url->url);
    printf("  HOST: %s\n", url->host);
    printf("  URI : %s\n", url->uri);
    printf("  PSTR: %s\n", url->pstr);
    printf(" PARAM: %d\n", url->params.pnumb);

    for (i = 0; i < url->params.pnumb; i++) {
        printf("    %2d: %s = %s\n", i, url->params.pkeys[i], url->params.pvals[i]); 
    }
    
}

/* End of file */
