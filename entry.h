#ifndef _ENTRY_H_
#define _ENTRY_H_

#include "stdint.h"

/* an entry can be a cache entry or a policy */
struct entry {
    /* source ip address and netmask length */
    uint32_t from;
    uint8_t from_pre;
    
    /* destination ip address and netmask length */
    uint32_t to;
    uint8_t to_pre;

    /* status information or acton */
    uint8_t desc;
};

#endif /* _ENTRY_H_ */
