#ifndef __HIJACK_H__
#define __HIJACK_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "naga_types.h"
#include "boots.h"


#define MAX_IPSTR_LEN  20
#define MAX_HIJACK_LOCATE_STR  64
#define MAX_HIJACK_STR 512
#define MAX_KEY_NUM 5

#define HIJACK_RULE_NUM_MAX     50
#define HIJACK_RULE_EFFECTIVE   1
#define HIJACK_RULE_UNEFFECTIVE 0




typedef enum {
    HIJACK_NULL_MODE,
    HIJACK_KEY_MODE,
    HIJACK_URL_MODE,
    HIJACK_GLOBAL_MODE,
    HIJACK_COMB_MODE,
    HIJACK_ROLL_MODE,
    HIJACK_URL_PCRE_MODE,
    HIJACK_MAX_MODE
} HIJACK_E;


typedef struct {
    uint8_t ip_str[MAX_IPSTR_LEN];
    uint32_t ip_str_len;
    time_t pri_time;
    naga_acl_t acl;
} hijack_ip_t;


typedef struct {
	int weight;
    char key[MAX_HIJACK_STR];
    naga_acl_t acl;
} roll_Key_t;


typedef struct {
    HIJACK_E mode;
	uint8_t key_num;
    uint32_t index;
    char host[MAX_HOST_LEN];
	char key[MAX_HIJACK_STR];
    roll_Key_t key_arry[MAX_KEY_NUM];
    char locate[MAX_HIJACK_LOCATE_STR];
	char val1[MAX_HIJACK_STR];
	char val2[MAX_HIJACK_STR];
    naga_acl_t acl;
} hijack_rule_t;

typedef struct{
    uint8_t effective;
    hijack_rule_t hijack;
} hijack_entry_t;


berr hijack_enable_set(int status);
berr hijack_enable_get(int *status);


berr ip_pkt_set_interval(int interval);
berr ip_set_interval(int interval);
berr pkt_set_interval(int interval);
berr ip_time_set_interval(int interval);
berr ip_time_interval_get(int *interval);
berr ip_interval_get(int *interval);
berr ip_pkt_interval_get(int *interval);
berr pkt_interval_get(int *interval);
berr hijack_add(hijack_rule_t *hijack);
berr hijack_del(uint32_t index);
berr hijack_get(uint32_t index, hijack_rule_t *hijack, uint8_t *effect);
berr hijack_clear_stat(uint32_t index);
berr log_path_set(const char *path);
void hijack_init(void);

berr
hijack_ip_init(uint32_t number);

berr
hijack_ip_add(hijack_ip_t *entry);

berr
hijack_ip_del(char *host);

hijack_ip_t *hijack_ip_get(char *key);
hijack_entry_t *get_hijack_table_ptr(void);
time_t *hijack_get_start_time(void);


#define api_hijack_enable_set       hijack_enable_set
#define api_hijack_enable_get       hijack_enable_get
#define api_ip_pkt_set_interval     ip_pkt_set_interval
#define api_ip_set_interval         ip_set_interval
#define api_pkt_set_interval        pkt_set_interval
#define api_ip_interval_get         ip_interval_get
#define api_ip_pkt_interval_get     ip_pkt_interval_get
#define api_pkt_interval_get        pkt_interval_get
#define api_ip_time_set_interval    ip_time_set_interval
#define api_ip_time_interval_get    ip_time_interval_get
#define api_hijack_add              hijack_add
#define api_hijack_del              hijack_del
#define api_hijack_get              hijack_get
#define api_hijack_ip_get           hijack_ip_get
#define api_hijack_ip_add           hijack_ip_add
#define api_hijack_ip_del           hijack_ip_del
#define api_hijack_ip_clear         hijack_ip_clear
#define api_get_hijack_table_ptr    get_hijack_table_ptr
#define api_hijack_get_start_time   hijack_get_start_time
#define api_hijack_clear_stat       hijack_clear_stat
#define api_log_path_set            log_path_set
#define api_log_pash_get		    log_path_get

#endif
