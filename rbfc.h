#ifndef _RBFC_H
#define _RBFC_H

#include "entry.h"
#include <tr1/cstdint>

void dump_rbfc();

/* init tss before use */
void init_rbcache();
/* free tss before exit */
void free_rbcache();

/* add a new cache entry */
void add_rbcache(uint32_t, uint32_t, uint8_t);
/* delete an entry */
void del_rbcache(uint32_t, uint32_t);
/* get an entry using the given ip addresses */
uint8_t get_rbcache(uint32_t, uint32_t);
/* update the cache using the given entry */
uint32_t update_rbcache(struct entry *);

#endif
