#include "pid.h"

berr naga_pid(hytag_t *hytag)
{
	struct pbuf *p = NULL;

	p = &hytag->pbuf;
   printf("zzzzzzzzzzzzzzzzz\n");
	return pid_ethernet(p, hytag);
}



