#ifndef __NAGA_ACL_H__
#define __NAGA_ACL_H__

#define ACT_DROP        1
#define ACT_FORWARD     2
#define ACT_LOG         4
#define ACT_HTTP_RESPONSE       8

typedef struct {
    uint32_t actions;
    uint32_t fwd_port;  
    uint64_t cnt;
} naga_acl_t;

#define ACL_DORP(_acl) \
    (_acl).actions |= ACT_DROP

#define ACL_FWD(_acl, _port) \
{ \
    (_acl).actions |= ACT_FORWARD; \
    (_acl).frd_port = _port; \
}

#define ACL_LOG(_acl) \
    (_acl).actions |= ACT_LOG

#define ACL_CNT(_acl) \
    (_acl).actions |= ACT_COUNT

#define ACL_HIT(_acl) \
    bts_atomic64_inc((_acl).cnt)

#endif /* __NAGA_ACL_H__ */
