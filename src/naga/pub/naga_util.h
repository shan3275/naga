#ifndef __NAGA_UTIL_H__
#define __NAGA_UTIL_H__

#include "boots.h"

#define NAGA_ACL_STR_SZ  1500

berr naga_acl_parse(const char *argv[], int argc, naga_acl_t *acl);
void naga_acl_string(naga_acl_t *acl, char *str);

void naga_url_dump(naga_url_t *url);

#endif /* !__NAGA_UTIL_H__ */
