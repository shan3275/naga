#ifndef __URL_H_
#define __URL_H_

#include "pcre.h"

#define MAX_UA_RULE   16

struct pcre_s
{
    int id;
    int used;
    char *pattern; 
    char *cli_pattern;
    pcre *cre;
    naga_acl_t acl;
};


typedef struct
{
    uint32_t inuse;
    struct pcre_s ua_pcre[MAX_UA_RULE] ;  
}ua_t;

berr naga_ua(hytag_t *hytag);
berr ua_rule_add(uint32_t id, char *url,  char *cli_url, naga_acl_t *acl);
berr ua_rule_del(uint32_t id);
struct pcre_s * ua_rule_get(uint32_t id);

#endif
