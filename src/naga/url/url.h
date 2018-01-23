#ifndef __URL_H_
#define __URL_H_

#include "pcre.h"

#define MAX_URL_RULE   128


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
    struct pcre_s url_pcre[MAX_URL_RULE] ;  
}url_t;

berr  naga_ori_url(hytag_t *hytag);
berr  naga_ref_url(hytag_t *hytag);

berr ori_url_rule_add(uint32_t id, char *url,  char *cli_url, naga_acl_t *acl);
berr ref_url_rule_add(uint32_t id, char *url,  char *cli_url, naga_acl_t *acl);

berr ori_url_rule_del(uint32_t id);
berr ref_url_rule_del(uint32_t id);

struct pcre_s * ori_url_rule_get(uint32_t id);
struct pcre_s * ref_url_rule_get(uint32_t id);


#endif
