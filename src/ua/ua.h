#ifndef __PCRE_S__
#define __PCRE_S__

#include "pcre.h"

struct pcre_s
{
    int id;
    int used;
    char *pattern; 
    char *cli_pattern;
    pcre *cre;
    naga_acl_t acl;
};

#endif /* end of __PCRE_S__ */


#ifndef __URL_H_
#define __URL_H_

#define MAX_UA_RULE   16

typedef struct
{
    uint32_t inuse;
    struct pcre_s ua_pcre[MAX_UA_RULE] ;  
}ua_t;

berr naga_ua(hytag_t *hytag);
berr ua_rule_add(uint32_t id, char *url,  char *cli_url, naga_acl_t *acl);
berr ua_rule_del(uint32_t id);
struct pcre_s * ua_rule_get(uint32_t id);

#endif /* end of __URL_H_ */
