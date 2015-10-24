#ifndef __ADP_CMD_H
#define __ADP_CMD_H


#include "vty.h"
void cmdline_adp_init(void);

berr adp_clear_interval();
void adp_cmd_config_write(struct vty *vty);

#endif