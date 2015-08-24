#ifndef __BOOTS_CUSTOM_H__
#define __BOOTS_CUSTOM_H__

#define MOD_CUSTOM \
    MOD_AMD, \
    MOD_DPF, \
    MOD_ITF, \
    MOD_PID, \
    MOD_VSR,

#define MOD_ARRAY_CUSTOM \
    {MOD_AMD, "AMD", "Admin Main Daemon"}, \
    {MOD_DPF, "DPF", "Data process flow"}, \
    {MOD_ITF, "ITF", "Interface module"}, \
    {MOD_PID, "PID", "Packet identify module"}, \
    {MOD_VSR, "VSR", "Visted recored module"}, 

#define DOPT_CUSTOM \
    DOPT_AMD, \
    DOPT_DPF, \
    DOPT_ITF, \
    DOPT_PID, \
    DOPT_VSR, 

#define DOPT_ARRAY_CUSTOM \
    {DOPT_AMD, OFF, "amd", "Admin manage daemon module"}, \
    {DOPT_DPF, OFF, "dpf", "data processs flow module"}, \
    {DOPT_ITF, OFF, "itf", "interface module"}, \
    {DOPT_PID, OFF, "pid", "packet identify module"}, \
    {DOPT_VSR, OFF, "vsr", "visited recored module"},

#define CNT_CUSTOM \
    /* VSR Module counters */ \
    VSR_PKTS, /* VSR module input packets*/ \
    VSR_UNURLPKTS, /* take no url packet number */ \
    VSR_URLPKTS, /* take url packet number */ \
    VSR_UNMATCHPKTS, /* not match packets */ \
    VSR_MATCHPKTS, /* match packets */ \
    VSR_RULE_MATCHPKTS, /* rule ip, url all match */

#define CNT_DEC_CUSTOM \
    /* VSR Module */ \
    CNT_DEC(VSR_PKTS), \
    CNT_DEC(VSR_UNURLPKTS), \
    CNT_DEC(VSR_URLPKTS), \
    CNT_DEC(VSR_UNMATCHPKTS), \
    CNT_DEC(VSR_MATCHPKTS), \
    CNT_DEC(VSR_RULE_MATCHPKTS),

/* Custom spinlock */
#define bts_spinlock_t rte_spinlock_t
#define bts_spin_lock(_lock) rte_spinlock_lock(_lock)
#define bts_spin_unlock(_lock) rte_spinlock_unlock(_lock)
#define bts_spin_init(_lock) rte_spinlock_init(_lock)

/* Custom automic opreation */
#define bts_atomic32_inc  rte_atomic32_inc
#define bts_atomic64_inc  rte_atomic64_inc
#define bts_atomic32_set  rte_atomic32_set
#define bts_atomic64_set  rte_atomic64_set

#endif /* ! __BOOTS_CUSTOM_H__ */
