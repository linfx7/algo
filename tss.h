#ifndef _TSS_H
#define _TSS_H

#include "entry.h"
#include <stdint.h>
#include <tr1/unordered_map>

/* default size of hashtables */
#define HT_SIZE 4096

/* restore all entries with same netmask lengthes */
struct tuple
{
    uint8_t from_pre, to_pre;
    std::tr1::unordered_map<uint64_t, uint8_t> ht;
    uint32_t ht_size;
};

void dump();

/* init tss before use */
void init_tss();
/* free tss before exit */
void free_tss();

/* add new tuple */
void add_tuple(uint8_t, uint8_t);
/* delete a tuple */
void del_tuple(uint8_t, uint8_t);

/* add a new entry */
void add_entry(struct entry *);
/* delete an entry */
void del_entry(struct entry *);
/* get an entry using the given ip addresses */
uint8_t get_entry(uint32_t, uint32_t);

/* generate a key */
uint64_t get_key(uint32_t, uint32_t);

uint8_t match(struct entry *, uint32_t, uint32_t);

#endif
