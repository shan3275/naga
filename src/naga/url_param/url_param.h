#ifndef __URL_PARAM_H_
#define __URL_PARAM_H_


#define MAX_URL_RULE   64

naga_url_param_rule_t
typedef struct {
    uint32_t id;
    uint32_t inuse;
    uint32_t pcnt;
    char keys[URL_PARAM_NUMB_MAX][URL_PARAM_KEY_LEN_MAX]; 
    char vals[URL_PARAM_NUMB_MAX][URL_PARAM_KEY_LEN_MAX]; 
    naga_acl_t acl;
} naga_url_param_rule_t;


#endif
