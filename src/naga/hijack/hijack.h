#ifndef __HIJACK_H__
#define __HIJACK_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "naga_types.h"
#include "boots.h"


#define MAX_IPSTR_LEN  20
#define MAX_HIJACK_STR 512

#define HIJACK_RULE_NUM_MAX     50
#define HIJACK_RULE_EFFECTIVE   1
#define HIJACK_RULE_UNEFFECTIVE 0




typedef enum {
    HIJACK_KEY_MODE,
    HIJACK_URL_MODE,
    HIJACK_MAX_MODE
} HIJACK_E;


typedef struct {
    uint8_t ip_str[MAX_IPSTR_LEN];
    uint32_t ip_str_len;
    naga_acl_t acl;
} hijack_ip_t;

typedef struct {
    HIJACK_E mode;
    uint32_t index;
    char host[MAX_HOST_LEN];
    char key[MAX_HIJACK_STR];
    char locate[MAX_HIJACK_STR];
} hijack_rule_t;

typedef struct{
    uint8_t effective;
    hijack_rule_t hijack;
} hijack_entry_t;


berr ip_num_set_interval(int interval);
berr ip_set_interval(int interval);
berr hijack_add(hijack_rule_t *hijack);
berr hijack_del(uint32_t index);
berr hijack_get(uint32_t index, hijack_rule_t *hijack, uint8_t *effect);
void hijack_init(void);

berr
hijack_ip_init(uint32_t number);

berr
hijack_ip_add(hijack_ip_t *entry);

berr
hijack_ip_del(char *host);

hijack_ip_t *hijack_ip_get(char *key);
hijack_entry_t *get_hijack_table_ptr(void);

#define api_ip_num_set_interval  ip_num_set_interval
#define api_ip_set_interval      ip_set_interval
#define api_url_replace_set      url_replace_set


#define api_hijack_add           hijack_add
#define api_hijack_del           hijack_del
#define api_hijack_get           hijack_get
#define api_hijack_ip_get        hijack_ip_get
#define api_hijack_ip_add        hijack_ip_add
#define api_hijack_ip_del        hijack_ip_del
#define api_get_hijack_table_ptr get_hijack_table_ptr


#endif