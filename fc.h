#ifndef _FC_H
#define _FC_H

#include "entry.h"
#include <tr1/cstdint>

void dump_fc();

/* init tss before use */
void init_cache();
/* free tss before exit */
void free_cache();

/* add a new cache entry */
void add_cache(uint32_t, uint32_t, uint8_t);
/* delete an entry */
void del_cache(uint32_t, uint32_t);
/* get an entry using the given ip addresses */
uint8_t get_cache(uint32_t, uint32_t);
/* update the cache using the given entry */
void update_cache(struct entry *);

#endif
