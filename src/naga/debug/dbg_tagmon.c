#include "naga_debug.h"

#define HYTAG_U16_DUMP(_tag, _field) { \
    printf("HYTAG[%d] %s: %.4d(0x%.4x)\n", rte_lcore_id(), #_field, _tag->_field, _tag->_field); \
}

#define HYTAG_U32_DUMP(_tag, _field) { \
    printf("HYTAG[%d] %s: %.8d(0x%.8x)\n", rte_lcore_id(), #_field, _tag->_field, _tag->_field); \
}

#define HYTAG_IP4_DUMP(_tag, _field) { \
    char _ipstr[IP4_STR_SZ]; \
    ip4_string(_tag->_field, _ipstr); \
    printf("HYTAG[%d] %s: %s\n", rte_lcore_id(), #_field, _ipstr); \
}

#define HYTAG_IP6_DUMP(_tag, _field) { \
    char _ipstr[IP6_STR_SZ]; \
    ip6_string(_tag->_field, _ipstr); \
    printf("HYTAG[%d] %s: %s\n", rte_lcore_id(), #_field, _ipstr); \
}

#define HYTAG_STR_DUMP(_tag, _field) { \
    printf("HYTAG[%d] %s: %s\n", rte_lcore_id(), #_field, _tag->_field); \
}

void hytag_print(hytag_t *tag)
{
    HYTAG_IP4_DUMP(tag, outer_srcip4);
    HYTAG_IP4_DUMP(tag, outer_dstip4);
    HYTAG_IP4_DUMP(tag, inner_srcip4);
    HYTAG_IP4_DUMP(tag, inner_dstip4);    

    HYTAG_IP6_DUMP(tag, outer_srcip6);
    HYTAG_IP6_DUMP(tag, outer_dstip6);
    HYTAG_IP6_DUMP(tag, inner_srcip6);
    HYTAG_IP6_DUMP(tag, inner_dstip6);

    HYTAG_U32_DUMP(tag, teid);

    HYTAG_U16_DUMP(tag, outer_srcport);
    HYTAG_U16_DUMP(tag, outer_dstport);
    HYTAG_U16_DUMP(tag, inner_srcport);
    HYTAG_U16_DUMP(tag, inner_dstport);

    HYTAG_STR_DUMP(tag, url);
}

/* End of file */
