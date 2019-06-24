#ifndef __URL_CMD_H
#define __URL_CMD_H

#define ORL_EXPR "origin url expr\n"
#define URL_EXPR "url expr\n"
#define ADD_EXPR "add action\n" 
#define DEL_EXPR "del action\n" 
#define URL_INDEX_EXPR "url index expr\n"
#define URL_PCRE_EXPR  "url pcre rule expr\n"
#define URL_ACT_EXPR   "url action expr\n"
#define URL_PARAM_EXPR "url other paramet\n"
#define URL_RATE_EXPR "url redir rate\n"

void cmdline_url_init(void);



void url_cmd_config_write(struct vty *vty);


#endif
