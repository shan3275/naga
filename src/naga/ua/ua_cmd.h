#ifndef __UA_CMD_H
#define __UA_CMD_H

#define UA_EXPR "ua expr\n"
#define SHOW_UA_EXPR "show ua expr\n"
#define ADD_EXPR "add action\n" 
#define DEL_EXPR "del action\n" 
#define UA_INDEX_EXPR "url index expr\n"
#define UA_ALL_EXPR   "url all expr\n"
#define UA_PCRE_EXPR  "url pcre rule expr\n"
#define UA_ACT_EXPR   "url action expr\n"
#define UA_PARAM_EXPR "url other paramet\n"
#define UA_RATE_EXPR "url redir rate\n"

void cmdline_ua_init(void);
void ua_cmd_config_write(struct vty *vty);

#endif
