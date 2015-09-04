#ifndef __BOOTS_CUSTOM_H__
#define __BOOTS_CUSTOM_H__

#define MOD_CUSTOM \
    MOD_AMD, \
    MOD_DPF, \
    MOD_ITF, \
    MOD_PID, \
    MOD_VSR

#define MOD_ARRAY_CUSTOM \
    {MOD_AMD, "AMD", "Admin Main Daemon"}, \
    {MOD_DPF, "DPF", "Data process flow"}, \
    {MOD_ITF, "ITF", "Interface module"}, \
    {MOD_PID, "PID", "Packet identify module"}, \
    {MOD_VSR, "VSR", "Visted recored module"}

#define DOPT_CUSTOM \
    DOPT_AMD, \
    DOPT_DPF, \
    DOPT_ITF, \
    DOPT_PID, \
    DOPT_VSR

#define DOPT_ARRAY_CUSTOM \
    {DOPT_AMD, OFF, "amd", "Admin manage daemon module"}, \
    {DOPT_DPF, OFF, "dpf", "data processs flow module"}, \
    {DOPT_ITF, OFF, "itf", "interface module"}, \
    {DOPT_PID, OFF, "pid", "packet identify module"}, \
    {DOPT_VSR, OFF, "vsr", "visited recored module"}

#define CNT_CUSTOM \
    /* VSR Module counters */ \
    VSR_PKTS, /* VSR module input packets*/ \
    VSR_UNURLPKTS, /* take no url packet number */ \
    VSR_URLPKTS, /* take url packet number */ \
    VSR_UNMATCHPKTS, /* not match packets */ \
    VSR_MATCHPKTS, /* match packets */ \
    VSR_RULE_MATCHPKTS, /* rule ip, url all match */ \
    VSR_RULE_LEN_OVERFLOW, /* url len overflow */ \
    VSR_RULE_URL_OVERFLOW,    /* url overflow */ \
    VSR_RULE_NEW_URL

#define CNT_DEF_CUSTOM \
    /* VSR Module */ \
    CNT_DEF(VSR_PKTS), \
    CNT_DEF(VSR_UNURLPKTS), \
    CNT_DEF(VSR_URLPKTS), \
    CNT_DEF(VSR_UNMATCHPKTS), \
    CNT_DEF(VSR_MATCHPKTS), \
    CNT_DEF(VSR_RULE_MATCHPKTS), \
    CNT_DEF(VSR_RULE_LEN_OVERFLOW), \
    CNT_DEF(VSR_RULE_URL_OVERFLOW), \
    CNT_DEF(VSR_RULE_NEW_URL)

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
