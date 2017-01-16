#ifndef _TSS_H_
#define _TSS_H_

#include "entry.h"
#include "types.h"
#include "hashtable.h"

/* default size of hashtables */
#define HT_SIZE 4096

struct t_hashtable
{
    struct hlist_head hhead[HT_SIZE];
};

/* restore all entries with same netmask lengthes */
struct tuple
{
    struct t_hashtable *ht;
    uint32_t ht_size;
};

struct tuple_entry
{
    struct hlist_node hn;
    struct entry *e;
};

/* init tss before use */
void init_tss();
/* free tss before exit */
void free_tss();

/* init a tuple */
void init_tuple(struct tuple *);
/* free a tuple */
void free_tuple(struct tuple *);

/* add a new entry */
void add_entry(struct entry *);
void add_tuple_entry(struct tuple_entry *);
/* delete an entry */
void del_entry(struct entry *);
void del_tuple_entry(struct tuple_entry *);
/* get an entry using the given ip addresses */
struct entry *get_entry(uint32_t, uint32_t);
struct tuple_entry *get_tuple_entry(uint32_t, uint32_t);
/* update caches using a policy */
void update_cache(struct entry *);

/* generate a key */
uint64_t get_key(uint32_t, uint32_t);

uint32_t masks[] = {0x00000000,
    0x80000000, 0xc0000000, 0xe0000000, 0xf0000000,
    0xf8000000, 0xfc000000, 0xfe000000, 0xff000000,
    0xff800000, 0xffc00000, 0xffe00000, 0xfff00000,
    0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000,
    0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000,
    0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00,
    0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0,
    0xfffffff8, 0xfffffffc, 0xfffffffe, 0xffffffff,
    };

int match(struct entry *, uint32_t, uint32_t);

#endif
