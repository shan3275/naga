#ifndef __BOOTS_H__
#define __BOOTS_H__

typedef unsigned char  int8_t;
typedef unsigned short int16_t;
typedef unsigned int   int32_t;
typedef unsigned long long int64_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char mac_t[6];
typedef u32_t ipv4_t;
typedef u16_t ipv6_t[8];

#include "boots_custom.h"

typedef enum { 
    MOD_DBG,
    MOD_CUSTOM,
    MOD_MAX
} bmod;

typedef struct {
    bmod mod;
    char *abbr;
    char *desc;
} mod_t;

typedef enum {
    E_SUCCESS,  // ??
    E_FAIL,     // ??
    E_NULL,     // ????
    E_MATCH,    // ???
    E_MEMORY,   // ???? 
    E_PARAM,    // ????
    E_EXCEED,   // ?? 
    E_OPEN,     // ????
    E_FOUND,    // ???
    E_EXIST,    // ????
    E_BUSY,     // ?? 
    E_MAX,      // ???????
} berr;

char *berr_array[E_MAX] = {
    "SUCCESS",
    "FAIL",
    "NULL",
    "MATCH",    // ???
    "MEMORY",   // ???? 
    "PARAM",    // ????
    "EXCEED",   // ?? 
    "OPEN",     // ????
    "FOUND",    // ???
    "EXIST",    // ????
    "BUSY",     // ?? 
    "UNKOWN"
};

mod_t mod_array[MOD_MAX] = {
    {MOD_DBG, "dbg", "debug module"},
#ifdef MOD_ARRAY_CUSTOM
    MOD_ARRAY_CUSTOM
#endif
} ;



typedef struct {
    u8_t major;
    u8_t minor;
    u16_t revison;
} version_t;

#endif
