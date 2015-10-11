#ifndef __BOOTS_CUSTOM_H__
#define __BOOTS_CUSTOM_H__

#define MOD_CUSTOM \
    MOD_AMD, \
    MOD_DPF, \
    MOD_ITF, \
    MOD_PID, \
    MOD_VSR, \
    MOD_DMR, \
    MOD_ACR, \
    MOD_ADP

#define MOD_ARRAY_CUSTOM \
    {MOD_AMD, BAS, OFF, OFF, "AMD", "Admin Main Daemon"}, \
    {MOD_DPF, BAS, OFF, OFF, "DPF", "Data process flow"}, \
    {MOD_ITF, BAS, OFF, OFF, "ITF", "Interface module"}, \
    {MOD_PID, ON,  OFF, OFF, "PID", "Packet identify module"}, \
    {MOD_VSR, ON,  OFF, OFF, "VSR", "Visted recored module"}, \
    {MOD_DMR, ON,  OFF, OFF, "DMR", "Domain rule module"}, \
    {MOD_ACR, ON,  OFF, OFF, "ACR", "Account rule module"}, \
    {MOD_ADP, ON,  OFF, OFF, "ADP", "Advertising push"}

#define DOPT_CUSTOM \
    DOPT_TAGMON, \
    DOPT_FAILPKT,\
    DOPT_LOG
	

#define DOPT_ARRAY_CUSTOM \
    {DOPT_TAGMON, OFF,  "tagmon", "Hytag monitor"}, \
    {DOPT_FAILPKT,OFF, "failpkt", "Process fail packet record"},\
    {DOPT_LOG, OFF, "log", "log record"}

#define CNT_CUSTOM \
    /* VSR Module counters */ \
    VSR_PKTS, /* VSR module input packets*/ \
    VSR_UNURLPKTS, /* take no url packet number */ \
    VSR_URLPKTS, /* take url packet number */ \
    VSR_UNMATCHPKTS, /* not match packets */ \
    VSR_MATCHPKTS, /* match packets */ \
    VSR_RULE_MATCHPKTS, /* rule ip, url all match */ \
    \
    DMR_PKTS, /* DMR module input packets*/\
    DMR_RULE_MATCH,  /* Match in host rule */\
    DMR_RULE_UNMATCH,/* Unmatch found in host rule*/\
    \
    ACR_PKTS, /* ACR module input packets*/ \
    ACR_TABLE_FOUND, /* Found in account table */ \
    ACR_TABLE_NOTFOUND, /* Not found in account table*/ \
    ACR_RULE_MATCH, /* Match in account rule */ \
    ACR_RULE_UNMATCH, /* Unmatch found in account rule*/ \
    \
    VSR_RULE_NEW_URL, \
    VSR_RULE_LEN_OVERFLOW, /* url len overflow */ \
    VSR_RULE_URL_OVERFLOW,    /* url overflow */ \
    \
    ADP_IPKTS, \
    ADP_DROP_PARAM, \
    ADP_DROP_GET_OR_POST, \
    ADP_DROP_121ZOU, \
    ADP_DROP_NOT_HAO123, \
    ADP_DROP_ACT_PUSH, \
    ADP_DROP_OUR_SUFFIX, \
    ADP_DROP_BACKSLASH_SUFFIX, \
    ADP_DROP_HTML_SUFFIX, \
    ADP_DROP_ADP_INTERVAL, \
    ADP_DROP_HEAD_GEN1, \
    ADP_DROP_SEND_PACKET1, \
    ADP_DROP_HEAD_GEN2, \
    ADP_DROP_SEND_PACKET2, \
    ADP_PUSH_PC, \
    ADP_PUSH_MOBILE, \
    ADP_PUSH_SUCCESS, \
    ADP_HAO123, \
    \
    NAGA_CNT_MAX

#define CNT_DEF_CUSTOM \
    /* VSR Module */ \
    CNT_DEF(VSR_PKTS), \
    CNT_DEF(VSR_UNURLPKTS), \
    CNT_DEF(VSR_URLPKTS), \
    CNT_DEF(VSR_UNMATCHPKTS), \
    CNT_DEF(VSR_MATCHPKTS), \
    CNT_DEF(VSR_RULE_MATCHPKTS), \
    \
    /* DMR Module */ \
    CNT_DEF(DMR_PKTS), \
    CNT_DEF(DMR_RULE_MATCH), \
    CNT_DEF(DMR_RULE_UNMATCH),\
    \
    /* ACR Module */ \
    CNT_DEF(ACR_PKTS), \
    CNT_DEF(ACR_TABLE_FOUND), \
    CNT_DEF(ACR_TABLE_NOTFOUND), \
    CNT_DEF(ACR_RULE_MATCH), \
    CNT_DEF(ACR_RULE_UNMATCH),\
    CNT_DEF(VSR_RULE_NEW_URL), \
    CNT_DEF(VSR_RULE_LEN_OVERFLOW), \
    CNT_DEF(VSR_RULE_URL_OVERFLOW), \
    \
    CNT_DEF(ADP_IPKTS), \
    CNT_DEF(ADP_DROP_PARAM), \
    CNT_DEF(ADP_DROP_GET_OR_POST), \
    CNT_DEF(ADP_DROP_121ZOU), \
    CNT_DEF(ADP_DROP_NOT_HAO123), \
    CNT_DEF(ADP_DROP_ACT_PUSH), \
    CNT_DEF(ADP_DROP_OUR_SUFFIX), \
    CNT_DEF(ADP_DROP_BACKSLASH_SUFFIX), \
    CNT_DEF(ADP_DROP_HTML_SUFFIX), \
    CNT_DEF(ADP_DROP_ADP_INTERVAL), \
    CNT_DEF(ADP_DROP_HEAD_GEN1), \
    CNT_DEF(ADP_DROP_SEND_PACKET1), \
    CNT_DEF(ADP_DROP_HEAD_GEN2), \
    CNT_DEF(ADP_DROP_SEND_PACKET2), \
    CNT_DEF(ADP_PUSH_PC), \
    CNT_DEF(ADP_PUSH_MOBILE), \
	CNT_DEF(ADP_PUSH_SUCCESS), \
    CNT_DEF(ADP_HAO123), \
    \
    CNT_DEF(NAGA_CNT_MAX)

/* Custom spinlock */
#define bts_spinlock_t rte_spinlock_t
#define bts_spin_lock(_lock) rte_spinlock_lock(_lock)
#define bts_spin_unlock(_lock) rte_spinlock_unlock(_lock)
#define bts_spin_init(_lock) rte_spinlock_init(_lock)

/* Custom automic opreation */
#define bts_atomic32_inc  rte_atomic32_inc
#define bts_atomic64_inc  rte_atomic64_inc
#define bts_atomic32_add  rte_atomic32_add
#define bts_atomic64_add  rte_atomic64_add
#define bts_atomic32_dec  rte_atomic32_dec
#define bts_atomic64_dec  rte_atomic64_dec
#define bts_atomic32_set  rte_atomic32_set
#define bts_atomic64_set  rte_atomic64_set
#define bts_atomic32_get  rte_atomic32_read
#define bts_atomic64_get  rte_atomic64_read

#endif /* ! __BOOTS_CUSTOM_H__ */
